/*
** main.c for huffman in /home/leroy_v/Perso/huffman
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Feb 23 17:24:41 2015 vincent leroy
** Last update Thu Feb 26 00:03:31 2015 vincent leroy
*/

#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "treetograph.h"
#include "predictsize.h"
#include "huffman.h"
#include "compress.h"
#include "decompress.h"

extern char *optarg;
extern int optind;

struct arg_s
{
    char in_filename[PATH_MAX];
    char out_filename[PATH_MAX];

    int compression;

    int return_code;
};

static void usage(const char *av0)
{
    printf("Usage: %s [-h] [-c | -d] -i input_file [-o output_file]\n", av0);
    printf("\t-h Display this help and exit\n");
    printf("\t-c Compress the input file\n");
    printf("\t-d Decompress the input file\n");
    printf("\t-i Input file name\n");
    printf("\t-o Output file name. If it is not precise then the output file name will be:\n");
    printf("\t\tinput_file.hc if compression is selected\n");
    printf("\t\tinput_file.hcd if decompression is selected\n");
}

static int parse_arg(int ac, char **av, struct arg_s *arg)
{
    int opt;

    memset(arg, 0, sizeof(struct arg_s));
    arg->compression = -1;
    while (arg->return_code == 0 && (opt = getopt(ac, av, "i:o:cdh")) != -1)
    {
        switch (opt)
        {
            case 'i':
                if (arg->in_filename[0] != '\0')
                {
                    fprintf(stderr, "You can only precise one input file at a time\n");
                    arg->return_code = 1;
                }
                strncpy(arg->in_filename, optarg, PATH_MAX);
                break;
            case 'o':
                if (arg->out_filename[0] != '\0')
                {
                    fprintf(stderr, "You can only precise one output file at a time\n");
                    arg->return_code = 1;
                }
                strncpy(arg->out_filename, optarg, PATH_MAX);
                break;
            case 'c':
            case 'd':
                if (arg->compression != -1)
                {
                    fprintf(stderr, "You must select compression or decompression not both\n");
                    arg->return_code = 1;
                }
                arg->compression = opt == 'c';
                break;
            case 'h':
                usage(av[0]);
                return 1;
            case '?':
                arg->return_code = 1;
                break;
            default:
                break;
        }
    }

    if (arg->return_code != 0)
        return arg->return_code;


    char in_path[PATH_MAX];
    char out_path[PATH_MAX];
    arg->return_code = 1;

    if (optind != ac)
    {
        fprintf(stderr, "You can only precise option\n");
        usage(av[0]);
    }
    else if (arg->in_filename[0] == '\0')
        fprintf(stderr, "You must precise the input filename\n");
    else if (arg->compression == -1)
        fprintf(stderr, "You must select compression or decompression\n");
    else if (realpath(arg->in_filename, in_path) == NULL)
        fprintf(stderr, "Error on input file '%s': %m\n", arg->in_filename);
    else if (realpath(arg->out_filename, out_path) == NULL)
    {
        if (errno != ENOENT)
            fprintf(stderr, "Error on input file '%s': %m\n", arg->out_filename);
        else
            arg->return_code = 0;
    }
    else if (strcmp(in_path, out_path) == 0)
        fprintf(stderr, "You cannot use the same file for input and output\n");
    else
        arg->return_code = 0;

    if (arg->out_filename[0] == '\0')
        snprintf(arg->out_filename, PATH_MAX, "%s.%s", arg->in_filename, arg->compression ? "hc" : "hcd");

    return arg->return_code;
}

static int open_and_map_file_read(const char *filename, uint8_t **mem)
{
    int fd;
    struct stat st;

    if (stat(filename, &st) == -1)
        return -1;
    if ((fd = open(filename, O_RDONLY)) == -1)
        return -1;
    *mem = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (*mem == MAP_FAILED)
        return -1;

    return st.st_size;
}

static int open_and_map_file_write(const char *filename, uint8_t **out_mem, int size)
{
    int fd;

    if ((fd = open(filename, O_RDWR | O_TRUNC | O_CREAT, 0644)) == -1)
        return -1;
    if (posix_fallocate(fd, 0, size) != 0)
    {
        close(fd);
        return -1;
    }
    *out_mem = mmap(NULL, size, PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    if (*out_mem == MAP_FAILED)
        return -1;

    return 0;
}

static int start_compression(const uint8_t *mem, int size, const char *out_filename)
{
    int ret = 1;
    int out_file_size;
    int header_size;
    uint8_t *out_mem;
    huffmantree_t *tree;

    if ((tree = get_tree_from_memory(mem, size)) == NULL)
        fprintf(stderr, "Unable to create the huffman tree: %m\n");
    else
    {
        header_size = get_full_header_size(tree);
        out_file_size = tree->predict_size_bytes + header_size;
        if (open_and_map_file_write(out_filename, &out_mem, out_file_size) == -1)
            fprintf(stderr, "Unable to open/create and map the file '%s': %m\n", out_filename);
        else
        {
            if (compress_memory(mem, size, tree, out_mem) == -1)
                fprintf(stderr, "Unable to compress the memory: %m\n");
            else
            {
                msync(out_mem, out_file_size, MS_SYNC);
                ret = 0;
            }
            munmap(out_mem, out_file_size);
        }
    }

    return ret;
}

static int start_decompression(const uint8_t *mem, int size, const char *out_filename)
{
    int ret = 1;
    uint8_t *out_mem;
    int out_file_size;

    out_file_size = predict_size_from_memory_byte(mem);
    if (out_file_size <= 0)
        fprintf(stderr, "Unable to predict the size of the output file (input file corrupted?)\n");
    else if (open_and_map_file_write(out_filename, &out_mem, out_file_size) == -1)
        fprintf(stderr, "Unable to open/create and map the file '%s': %m\n", out_filename);
    else
    {
        if (decompress_memory(mem, size, out_mem) == -1)
            fprintf(stderr, "Unable to decompress the memory: %m\n");
        else
        {
            msync(out_mem, out_file_size, MS_SYNC);
            ret = 0;
        }
        munmap(out_mem, out_file_size);
    }

    return ret;
}

// XXX: Don't forget the graph
int main(int ac, char **av)
{
    struct arg_s arg;
    uint8_t *mem;
    int in_file_size;
    int ret = 0;

    if (parse_arg(ac, av, &arg) != 0)
        return arg.return_code;

    if ((in_file_size = open_and_map_file_read(arg.in_filename, &mem)) == -1)
    {
        fprintf(stderr, "Unable to open and map the file '%s': %m\n", arg.in_filename);
        return 1;
    }

    if (arg.compression)
        ret = start_compression(mem, in_file_size, arg.out_filename);
    else
        ret = start_decompression(mem, in_file_size, arg.out_filename);

    munmap(mem, in_file_size);
    return ret;
}
