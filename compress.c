/*
** compress.c for huffman in /home/leroy_v/Perso/huffman
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Tue Feb 24 17:23:40 2015 vincent leroy
** Last update Wed Feb 25 23:51:39 2015 vincent leroy
*/

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "header.h"
#include "compress.h"
#include "predictsize.h"

static void fill_one_byte(const huffmannode_t *node, byte_to_bit_t tab[MAX_VALUE], byte_to_bit_t acc)
{
    if (node->left == NULL && node->right == NULL)
        tab[node->data] = acc;
    else
    {
        ++acc.nb_bits;

        if (node->left != NULL)
            fill_one_byte(node->left, tab, acc);
        if (node->right != NULL)
        {
            acc.bits |= 1 << (acc.nb_bits - 1);
            fill_one_byte(node->right, tab, acc);
        }
    }
}

static void tree_to_tab(const huffmannode_t *root, byte_to_bit_t tab[MAX_VALUE])
{
    byte_to_bit_t acc = {0, 0};
    fill_one_byte(root, tab, acc);
}

static void write_byte(uint8_t *out_mem, int off, uint8_t byte)
{
    if (off == 0)
        out_mem[0] = byte;
    else
    {
        out_mem[0] |= byte << off;
        out_mem[1] |= byte >> (8 - off);
    }
}

static void write_code(uint8_t *out_mem, int off, const byte_to_bit_t *code)
{
    int nb_bits;
    int i;

    for (nb_bits = code->nb_bits, i = 0; nb_bits > 0; nb_bits -= 8, i += 8)
    {
        uint8_t byte = (code->bits >> i) & 0xFF;
        write_byte(out_mem, off, byte);
        ++out_mem;
    }
}

static int write_header_node(const huffmannode_t *node, uint8_t *out_mem, int off)
{
    if (node->left == NULL && node->right == NULL)
    {
        memcpy(out_mem + off, &node->weight, sizeof(node->weight));
        memcpy(out_mem + off + sizeof(node->weight), &node->data, sizeof(node->data));
        return off + SIZE_LEAF_IN_FILE;
    }

    if (node->left != NULL)
        off = write_header_node(node->left, out_mem, off);
    if (node->right != NULL)
        off = write_header_node(node->right, out_mem, off);

    return off;
}

static void write_header_tree(const huffmantree_t *tree, uint8_t *out_mem)
{
    out_mem += write_header(out_mem, &tree->header);
    write_header_node(tree->root, out_mem, 0);
}

int compress(const char *filename, const huffmantree_t *tree, const char *out_filename)
{
    int ret = -1;
    int fdin;
    int fdout;
    uint64_t file_size;
    int header_size;
    struct stat st;
    uint8_t *mem;
    uint8_t *out_mem;

    if (stat(filename, &st) != -1)
    {
        fdin = open(filename, O_RDONLY);
        if (fdin != -1)
        {
            fdout = open(out_filename, O_RDWR | O_TRUNC | O_CREAT, 0644);
            if (fdout != -1)
            {
                mem = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fdin, 0);
                if (mem != MAP_FAILED)
                {
                    header_size = get_full_header_size(tree);
                    file_size = tree->predict_size_bytes + header_size;
                    out_mem = mmap(NULL, file_size, PROT_WRITE, MAP_SHARED, fdout, 0);
                    if (out_mem != MAP_FAILED)
                    {
                        if (posix_fallocate(fdout, 0, file_size) == 0)
                        {
                            ret = compress_memory(mem, st.st_size, tree, out_mem);
                            msync(out_mem, file_size, MS_SYNC);
                        }
                        munmap(out_mem, file_size);
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

int compress_memory(const uint8_t *mem, int size, const huffmantree_t *tree, uint8_t *out_mem)
{
    write_header_tree(tree, out_mem);
    out_mem += get_full_header_size(tree);

    byte_to_bit_t corres[MAX_VALUE] = {};
    tree_to_tab(tree->root, corres);

    uint64_t counter = 0;
    int i;
    for (i = 0; i < size; ++i)
    {
        write_code(out_mem + (counter / 8), counter % 8, &corres[mem[i]]);
        counter += corres[mem[i]].nb_bits;
    }

    return 0;
}
