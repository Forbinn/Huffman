/*
** decompression.c for huffman in /home/leroy_v/Perso/Huffman/src/decompression
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Tue Feb 24 21:41:22 2015 vincent leroy
** Last update Tue Apr 07 17:13:39 2015 vincent leroy
*/

#include <stdlib.h>

#include "decompression.h"

int decode_byte(const t_huffmantree *tree, const uint8_t *mem, int off, uint8_t *byte)
{
    int counter = 0;

    const t_huffmannode *node = tree->root;
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
