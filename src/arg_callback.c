/*
** arg_callback.c for src in /home/leroy_v/Perso/Huffman/src
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Sun Apr 05 16:28:32 2015 vincent leroy
** Last update Sat Apr 11 19:07:01 2015 vincent leroy
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <limits.h>

#include "arg_callback.h"

bool set_block_size(const char *option, const char *arg, void *userdata)
{
    static uint64_t page_size = 0;
    static uint64_t page_count = 0;

    if (arg == NULL)
        return false;

    uint32_t *block_size = (uint32_t*)userdata;
    char *endptr;
    uint64_t tmp = strtoll(arg, &endptr, 10);

    if (endptr == arg || tmp < 1)
    {
        if (option != NULL)
            fprintf(stderr, "The block size need to be a valid integer\n");
        return false;
    }

    if (strcmp(endptr, "kB") == 0)
        tmp *= 1024;
    else if (strcmp(endptr, "MB") == 0)
        tmp *= 1024 * 1024;
    else if (strcmp(endptr, "GB") == 0)
        tmp *= 1024 * 1024 * 1024;
    else
    {
        if (option != NULL)
            fprintf(stderr, "The block size suffix can be either kB or MB or GB\n");
        return false;
    }

    if (page_size == 0)
        page_size = sysconf(_SC_PAGESIZE);
    if (page_count == 0)
        page_count = sysconf(_SC_PHYS_PAGES);

    if ((tmp / page_size) >= page_count)
    {
        if (option != NULL)
            fprintf(stderr, "The block size requested is greater than the memory size\n");
        return false;
    }

    if (tmp > UINT_MAX)
    {
        if (option != NULL)
            fprintf(stderr, "The block size is too large: setting it to the maximum value: %u bytes\n", UINT_MAX);
        *block_size = UINT_MAX;
    }
    else
        *block_size = tmp;

    return true;
}

bool set_output_filename(const char *option, const char *arg, void *userdata)
{
    (void)option;

    const char **out_filename = (const char**)userdata;
    if (strlen(arg) == 0)
    {
        fprintf(stderr, "You need to precise an output filename\n");
        return false;
    }

    *out_filename = arg;
    return true;
}

bool set_thread_number(const char *option, const char *arg, void *userdata)
{
    if (arg == NULL)
        return false;

    int *number_of_thread = (int*)userdata;
    char *endptr;
    int tmp = strtol(arg, &endptr, 10);

    if (arg == endptr || tmp < 1)
    {
        if (option != NULL)
            fprintf(stderr, "The number of thread need to be a valid integer\n");
        return false;
    }

    *number_of_thread = tmp;
    return true;
}

bool set_memlimit(const char *option, const char *arg, void *userdata)
{
    if (arg == NULL)
        return false;

    uint64_t *max_memory = (uint64_t*)userdata;
    char *endptr;
    uint64_t tmp = strtoll(arg, &endptr, 10);

    if (endptr == arg || tmp < 1)
    {
        if (option != NULL)
            fprintf(stderr, "You need to precise a valid number for the memory size\n");
        return false;
    }

    if (strcmp(endptr, "kB") == 0)
        tmp *= 1024;
    else if (strcmp(endptr, "MB") == 0)
        tmp *= 1024 * 1024;
    else if (strcmp(endptr, "GB") == 0)
        tmp *= 1024 * 1024 * 1024;
    else
    {
        if (option != NULL)
            fprintf(stderr, "The maximum memory size suffix can be either kB or MB or GB\n");
        return false;
    }

    *max_memory = tmp;
    return true;
}

bool set_compress(const char *option, const char *arg, void *userdata)
{
    (void)option;
    (void)arg;

    int *compression = (int*)userdata;
    *compression = 1;
    return true;
}

bool set_decompress(const char *option, const char *arg, void *userdata)
{
    (void)option;
    (void)arg;

    int *compression = (int*)userdata;
    *compression = 0;
    return true;
}
