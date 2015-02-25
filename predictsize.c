/*
** predictsize.c for huffman in /home/leroy_v/Perso/huffman
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Tue Feb 24 16:55:00 2015 vincent leroy
** Last update Wed Feb 25 18:09:05 2015 vincent leroy
*/

#include <string.h>
#include <stdlib.h>

#include "header.h"
#include "predictsize.h"

static uint64_t real_predict_size(const huffmannode_t *node, int depth)
{
    if (node == NULL)
        return 0;

    if (node->left == NULL && node->right == NULL)
        return node->weight * depth;

    uint64_t weight = 0;
    if (node->left != NULL)
        weight += real_predict_size(node->left, depth + 1);
    if (node->right != NULL)
        weight += real_predict_size(node->right, depth + 1);

    return weight;
}

uint64_t predict_size_bit(const huffmannode_t *root)
{
    return real_predict_size(root, 0);
}

uint64_t predict_size_byte(const huffmannode_t *root)
{
    uint64_t bits = predict_size_bit(root);
    return bits / 8 + ((bits % 8) != 0);
}

uint64_t predict_size_from_memory_bit(const uint8_t *header)
{
    return predict_size_from_memory_byte(header) * 8;
}

uint64_t predict_size_from_memory_byte(const uint8_t *header)
{
    header_t h;
    uint32_t i;
    uint64_t bytes = 0;

    header += read_header(header, &h, NULL);
    for (i = 0; i < h.nb_leaf; ++i)
    {
        uint32_t weight;

        memcpy(&weight, header, sizeof(weight));
        header += SIZE_LEAF_IN_FILE;
        bytes += weight;
    }

    return bytes;
}
