/*
** io_operation.c for src in /home/leroy_v/Perso/Huffman/src
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Sun Apr 05 16:33:02 2015 vincent leroy
** Last update Sat Apr 25 20:28:45 2015 vincent leroy
*/

#include <stdlib.h>
#include <unistd.h>

#include "header.h"
#include "huffman.h"
#include "io_operation.h"

uint32_t read_raw_data(t_compress_arg *arg, void *userdata)
{
    int fd = *(int*)userdata;
    ssize_t size_read = read(fd, arg->mem, arg->mem_size);
    if (size_read == (ssize_t)-1)
        return(uint32_t)-1;

    arg->mem_size = size_read;
    return size_read;
}

uint32_t read_compressed_data(t_decompress_arg *arg, void *userdata)
{
    int fd = *(int*)userdata;
    uint8_t nb_leaf;
    uint16_t i;
    uint32_t size_read = 0;
    int byte_read;
    t_huffmannode *leafs[MAX_VALUE] = {};

    if ((byte_read = read_number(fd, &arg->crc, 32)) == 0)
        return 0; // EOF
    else if (byte_read < (int)sizeof(arg->crc))
        return -1;

    if (read_number(fd, &nb_leaf, 8) < (int)sizeof(nb_leaf))
        return -1;
    for (i = 0; i < (uint16_t)(nb_leaf) + 1; ++i)
    {
        uint32_t weight;
        uint8_t data;
        if (read_number(fd, &weight, 32) < (int)sizeof(weight))
            return -1;
        else if (read_number(fd, &data, 8) < (int)sizeof(data))
            return -1;

        t_huffmannode *node = create_node(NULL);
        node->weight = weight;
        node->data = data;
        leafs[node->data] = node;
    }
    arg->tree = get_tree_from_leaf(leafs);
    if (read_number(fd, &arg->padding, 8) < (int)sizeof(arg->padding))
        return -1;
    if (read_number(fd, &arg->mem_size, 32) < (int)sizeof(arg->mem_size))
        return -1;
    arg->mem = malloc(arg->mem_size * sizeof(uint8_t));
    while (size_read < arg->mem_size)
    {
        int32_t tmp = read(fd, arg->mem + size_read, arg->mem_size - size_read);
        if (tmp < 1)
        {
            arg->mem_size = size_read;
            return tmp;
        }
        size_read += tmp;
    }
    return 1;
}

uint32_t write_compressed_data(const t_compress_ret *ret, void *userdata)
{
    int fd = *(int*)userdata;
    uint16_t idx = 0;
    uint16_t i;
    uint32_t size_write = 0;
    const t_huffmannode *leafs[MAX_VALUE] = {};

    if (write_number(fd, &ret->crc, 32) == -1)
        return -1;
    tree_to_leafs(ret->tree->root, leafs, &idx);
    if (write_number(fd, &ret->tree->nb_leaf, 8) == -1)
        return -1;
    for (i = 0; i < idx; ++i)
    {
        if (write_number(fd, &leafs[i]->weight, 32) == -1)
            return -1;
        if (write_number(fd, &leafs[i]->data, 8) == -1)
            return -1;
    }
    if (write_number(fd, &ret->padding, 8) == -1)
        return -1;
    if (write_number(fd, &ret->mem_size, 32) == -1)
        return -1;
    while (size_write < ret->mem_size)
    {
        int32_t tmp = write(fd, ret->mem + size_write, ret->mem_size - size_write);
        if (tmp == -1)
            return -1;

        size_write += tmp;
    }
    return 0;
}

uint32_t write_raw_data(const t_decompress_ret *ret, void *userdata)
{
    int fd = *(int*)userdata;
    uint32_t size_write = 0;

    while (size_write < ret->mem_size)
    {
        int32_t tmp = write(fd, ret->mem + size_write, ret->mem_size - size_write);
        if (tmp == -1)
            return -1;

        size_write += tmp;
    }
    return 0;
}
