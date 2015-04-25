/*
** main.c for Huffman in /home/leroy_v/Perso/Huffman/src
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Tue Mar 31 20:35:41 2015 vincent leroy
** Last update Sat Apr 25 21:03:53 2015 vincent leroy
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <limits.h>

#include "huffman.h"
#include "thread.h"
#include "header.h"
#include "compression_threaded.h"
#include "decompression_threaded.h"
#include "io_operation.h"
#include "arg_callback.h"
#include "parse_arg.h"
#include "crc32.h"

static bool usage(const char *option, const char *arg, void *userdata)
{
    (void)option;
    (void)arg;

    char *av0 = (char*)userdata;

    printf("Usage: %s [option] [filename]\n", av0);
    printf("option can be one or more of these:\n");
    printf("\t-h,--help             Display this help and exit\n");
    printf("\t-v,--version          Print the version and exit\n");
    printf("\t-c,--compress         Compress the input file\n");
    printf("\t-d,--decompress       Uncompress the output file\n");
    printf("\t-o,--output filename  Precise an output filename (default: standard output)\n");
    printf("\t-t,--thread number    Uses a maximum of number thread (default: number of core)\n");
    printf("\t--block-size=size     Set the block size (default: 10MB)\n");
    printf("\t--memlimit=size       Set the maximum memory use to the value size (default: 100MB)\n");
    printf("\n");
    printf("All the size must be suffixed by kB, MB or GB\n");
    return false;
}

static bool version(const char *option, const char *arg, void *userdata)
{
    (void)option;
    (void)arg;

    char *av0 = (char*)userdata;

    printf("%s version %d.%d.%d\n", av0, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    return false;
}

static int open_in_out_file(const char *in_filename, char *out_filename, int *fd_in, int *fd_out, int compress)
{
    int allocation = 0;
    struct stat st;

    if (in_filename == NULL || strcmp(in_filename, "-") == 0)
        *fd_in = STDIN_FILENO;
    else
    {
        if (stat(in_filename, &st) == -1)
        {
            fprintf(stderr, "Unable to stat the input file '%s': %m\n", in_filename);
            return EXIT_FAILURE;
        }
        if (S_ISDIR(st.st_mode))
        {
            fprintf(stderr, "The input file refer to a directory: cannot %scompress it\n", compress ? "" : "de");
            return EXIT_FAILURE;
        }

        if ((*fd_in = open(in_filename, O_RDONLY)) == -1)
        {
            fprintf(stderr, "Unable to open the input file '%s': %m\n", in_filename);
            return EXIT_FAILURE;
        }
        if (out_filename == NULL)
        {
            allocation = 1;
            out_filename = calloc(strlen(in_filename) + 5, sizeof(char));
            if (compress)
                sprintf(out_filename, "%s.hc", in_filename);
            else
            {
                char *dot = strrchr(in_filename, '.');
                if (dot == NULL)
                {
                    fprintf(stderr, "Cannot guess the original name: use %s.out\n", in_filename);
                    sprintf(out_filename, "%s.out", in_filename);
                }
                else
                    strncpy(out_filename, in_filename, dot - in_filename);
            }
        }
    }

    if (out_filename == NULL)
        *fd_out = STDOUT_FILENO;
    else
    {
        if (stat(out_filename, &st) != -1 && S_ISDIR(st.st_mode))
        {
            close(*fd_in);
            fprintf(stderr, "The output file refer to a directory: cannot write to it\n");
            if (allocation)
                free(out_filename);
            return EXIT_FAILURE;
        }

        if ((*fd_out = open(out_filename, O_WRONLY | O_TRUNC | O_CREAT, 0644)) == -1)
        {
            close(*fd_in);
            fprintf(stderr, "Unable to open/create the output file '%s': %m\n", out_filename);
            if (allocation)
                free(out_filename);
            return EXIT_FAILURE;
        }
    }

    if ((compress && isatty(*fd_out)) || (!compress && isatty(*fd_in)))
    {
        if (compress)
            fprintf(stderr, "Cannot write compressed data to a terminal\n");
        else
            fprintf(stderr, "Cannot read compressed data from a terminal\n");
        close(*fd_in);
        close(*fd_out);
        if (allocation)
            free(out_filename);
        return EXIT_FAILURE;
    }

    if (allocation)
        free(out_filename);
    return EXIT_SUCCESS;
}

static void load_environment_variable(int *thread_number, uint32_t *block_size, uint64_t *max_memory)
{
    set_thread_number(NULL, getenv("HA_THREAD"), thread_number);
    set_block_size(NULL, getenv("HA_BLOCK_SIZE"), block_size);
    set_memlimit(NULL, getenv("HA_MEMLIMIT"), max_memory);
}

static int check_in_out_filename(const char *in_filename, const char *out_filename)
{
    if (in_filename == NULL || out_filename == NULL || strcmp(in_filename, "-") == 0)
        return EXIT_SUCCESS;

    char in_realpath[PATH_MAX];
    char out_realpath[PATH_MAX];

    if (realpath(in_filename, in_realpath) == NULL)
    {
        fprintf(stderr, "Unable to get the full path for the file '%s': %m\n", in_filename);
        return EXIT_FAILURE;
    }
    if (realpath(out_filename, out_realpath) != NULL)
        if (strcmp(in_realpath, out_realpath) == 0)
        {
            fprintf(stderr, "The input and output files cannot be the same\n");
            return EXIT_FAILURE;
        }

    return EXIT_SUCCESS;
}

char* bytes_to_standard_form(uint64_t bytes)
{
    static char buffer[1024] = {};
    static const uint64_t tera_bytes = 1024UL * 1024UL * 1024UL * 1024UL;
    static const uint64_t giga_bytes = 1024UL * 1024UL * 1024UL;
    static const uint64_t mega_bytes = 1024UL * 1024UL;
    static const uint64_t kilo_bytes = 1024UL;

    if (bytes >= tera_bytes)
        snprintf(buffer, 1024, "%.3f TB", ((double)bytes / (double)tera_bytes));
    else if (bytes >= giga_bytes)
        snprintf(buffer, 1024, "%.3f GB", ((double)bytes / (double)giga_bytes));
    else if (bytes >= mega_bytes)
        snprintf(buffer, 1024, "%.3f MB", ((double)bytes / (double)mega_bytes));
    else if (bytes >= kilo_bytes)
        snprintf(buffer, 1024, "%.3f kB", ((double)bytes / (double)kilo_bytes));
    else
        snprintf(buffer, 1024, "%lu B", bytes);

    return buffer;
}

int main(int ac, char **av)
{
    int thread_number = number_of_cores();
    uint32_t block_size = 10 * 1024 * 1024;
    uint64_t max_memory = 10 * block_size;
    int compression = 1;
    const char *filename = NULL;
    const char *out_filename = NULL;
    opt_error error;
    const opts options[] = {
        {'h',  "help",       NO_ARG,       &usage,               av[0]},
        {'v',  "version",    NO_ARG,       &version,             av[0]},
        {'o',  "output",     REQUIRED_ARG, &set_output_filename, &out_filename},
        {'t',  "thread",     REQUIRED_ARG, &set_thread_number,   &thread_number},
        {'c',  "compress",   NO_ARG,       &set_compress,        &compression},
        {'d',  "decompress", NO_ARG,       &set_decompress,      &compression},
        {'\0', "block-size", REQUIRED_ARG, &set_block_size,      &block_size},
        {'\0', "memlimit",   REQUIRED_ARG, &set_memlimit,        &max_memory},
        {'\0', NULL,         NO_ARG,       NULL,                 NULL}
    };

    load_environment_variable(&thread_number, &block_size, &max_memory);
    if (parse_arg(&ac, &av, options, &error) == -1)
    {
        switch (error.err_type)
        {
            case UNKNOWN_OPT:
                if (error.is_short_arg)
                    fprintf(stderr, "Unknown option: '%c'\n", av[error.argv_idx][error.idx]);
                else
                    fprintf(stderr, "Unknown option: '%s'\n", av[error.argv_idx] + error.idx);
                usage(NULL, NULL, av[0]);
                break;
            case ARG_NOT_PROVIDE:
                fprintf(stderr, "Option required a argument\n");
                usage(NULL, NULL, av[0]);
                break;
            default:
                break;
        }
        return EXIT_FAILURE;
    }

    if (max_memory < ((uint64_t)block_size * 2))
    {
        fprintf(stderr, "The max memory usage need to be at least twice greater than the block size\n");
        return EXIT_FAILURE;
    }

    if (ac > 1)
        filename = av[1];

    int ret;
    if ((ret = check_in_out_filename(filename, out_filename)) == EXIT_FAILURE)
        return ret;

    int fd_in;
    int fd_out;
    if ((ret = open_in_out_file(filename, (char*)out_filename, &fd_in, &fd_out, compression)) == EXIT_FAILURE)
        return ret;

    generate_crc32_table();

    t_header header;
    if (compression)
    {
        header_init(&header, block_size);
        header_write(&header, fd_out);

        t_c_thread_param thread_param = {
            .block_size = block_size,
            .number_of_thread = thread_number,
            .memory_max = max_memory
        };
        t_c_error c_error = compress_with_thread(&thread_param, &read_raw_data, &write_compressed_data, &fd_in, &fd_out);
        ret = EXIT_FAILURE;
        switch (c_error)
        {
            case C_READ_ERROR:
                fprintf(stderr, "Unable to read the input stream\n");
                break;
            case C_WRITE_ERROR:
                fprintf(stderr, "Unable to write to the output stream\n");
                break;
            default:
                ret = EXIT_SUCCESS;
        }
    }
    else
    {
        if (header_read(&header, fd_in) == -1)
        {
            fprintf(stderr, "Unable to read the header (input file corrupted?)\n");
            ret = EXIT_FAILURE;
        }
        else if (check_version(&header) == -1)
        {
            fprintf(stderr, "The version of this compressed file is not supported\n");
            ret = EXIT_FAILURE;
        }
        else
        {
            t_u_thread_param thread_param = {
                .block_size = header.block_size,
                .number_of_thread = thread_number,
                .memory_max = max_memory
            };
            t_d_error d_error = decompress_with_thread(&thread_param, &read_compressed_data, &write_raw_data, &fd_in, &fd_out);
            ret = EXIT_FAILURE;
            switch (d_error)
            {
                case D_READ_ERROR:
                    fprintf(stderr, "Unable to read the input stream\n");
                    break;
                case D_WRITE_ERROR:
                    fprintf(stderr, "Unable to write to the output stream\n");
                    break;
                case D_CRC_ERROR:
                    fprintf(stderr, "The crc did not match (input corrupted?)\n");
                    break;
                default:
                    ret = EXIT_SUCCESS;
            }
        }
    }

    close(fd_in);
    close(fd_out);
    if (ret == EXIT_FAILURE)
    {
        fprintf(stderr, "%sompression failed\n", compression ? "C" : "Dec");
        if (out_filename != NULL)
            unlink(out_filename);
    }

    return ret;
}
