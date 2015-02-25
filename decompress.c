/*
** decompress.c for huffman in /home/leroy_v/Perso/huffman
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Tue Feb 24 21:41:22 2015 vincent leroy
** Last update Wed Feb 25 18:12:31 2015 vincent leroy
*/

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "decompress.h"
#include "header.h"
#include "predictsize.h"

static int decode_byte(const huffmantree_t *tree, const uint8_t *mem, int off, uint8_t *byte)
{
    int counter = 0;

    const huffmannode_t *node = tree->root;
    while (node->left != NULL)
    {
        uint8_t mask = 1 << off;
        uint8_t bit = (mem[0] & mask) != 0;

        node = bit ? node->right : node->left;

        ++counter;
        ++off;
        if (off > 7)
        {
            off = 0;
            ++mem;
        }
    }

    *byte = node->data;
    return counter;
}

int decompress(const char *filename, const char *out_filename)
{
    int ret = -1;
    int fdin;
    int fdout;
    struct stat st;
    uint8_t *mem;
    uint8_t *out_mem;
    uint64_t predicted_size;

    if (stat(filename, &st) != -1)
    {
        if (st.st_size < 4)
            return ret;

        fdin = open(filename, O_RDONLY);
        if (fdin != -1)
        {
            fdout = open(out_filename, O_RDWR | O_TRUNC | O_CREAT, 0644);
            if (fdout != -1)
            {
                mem = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fdin, 0);
                if (mem != MAP_FAILED)
                {
                    predicted_size = predict_size_from_memory_byte(mem);
                    out_mem = mmap(NULL, predicted_size, PROT_WRITE, MAP_SHARED, fdout, 0);
                    if (out_mem != MAP_FAILED)
                    {
                        if (posix_fallocate(fdout, 0, predicted_size) == 0)
                        {
                            ret = decompress_memory(mem, st.st_size, out_mem);
                            msync(out_mem, predicted_size, MS_SYNC);
                        }
                        munmap(out_mem, predicted_size);
                    }
                    munmap(mem, st.st_size);
                }
                close(fdout);
            }
            close(fdin);
        }
    }

    return ret;
}

int decompress_memory(const uint8_t *mem, int size, uint8_t *out_mem)
{
    huffmannode_t *leafs[MAX_VALUE] = {};
    header_t header;
    huffmantree_t *tree;
    int ret;
    int i = 0;
    int off = 0;
    int out_counter = 0;
    int size_bits;

    ret = read_header(mem, &header, leafs);
    mem += ret;
    size -= ret;

    i = 0;
    tree = get_tree_from_leaf(leafs);
    size_bits = (size * 8) - header.padding_bits;
    while (i < size_bits)
    {
        i += decode_byte(tree, mem + (i / 8), off, out_mem + out_counter);
        off = i % 8;
        ++out_counter;
    }

    return 0;
}
