/*
** compression.c for huffman in /home/leroy_v/Perso/Huffman/src/compression
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Tue Feb 24 17:23:40 2015 vincent leroy
** Last update Tue Apr 07 17:18:29 2015 vincent leroy
*/

#include <string.h>

#include "compression.h"

static void fill_one_byte(const t_huffmannode *node, t_byte_to_bit tab[MAX_VALUE], t_byte_to_bit acc)
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

void tree_to_tab(const t_huffmantree *tree, t_byte_to_bit tab[MAX_VALUE])
{
    memset(tab, 0, sizeof(*tab) * MAX_VALUE);
    t_byte_to_bit acc = {0, 0};
    fill_one_byte(tree->root, tab, acc);
}

void write_code(uint8_t *out_mem, int off, const t_byte_to_bit *code)
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
