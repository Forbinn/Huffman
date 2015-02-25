/*
** header.c for huffman in /home/leroy_v/Perso/huffman
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Wed Feb 25 17:33:32 2015 vincent leroy
** Last update Wed Feb 25 18:10:01 2015 vincent leroy
*/

#include <string.h>

#include "header.h"

uint32_t get_header_size()
{
    return MEMBER_SIZE(header_t, padding_bits) + MEMBER_SIZE(header_t, nb_leaf);
}

int read_header(const uint8_t *mem, header_t *header, huffmannode_t *leafs[MAX_VALUE])
{
    uint32_t i;
    const uint8_t *save_mem = mem;

    memcpy(&header->padding_bits, mem, sizeof(header->padding_bits));
    mem += sizeof(header->padding_bits);
    memcpy(&header->nb_leaf, mem, sizeof(header->nb_leaf));
    mem += sizeof(header->nb_leaf);

    if (leafs == NULL)
        return mem - save_mem;

    for (i = 0; i < header->nb_leaf; ++i)
    {
        uint32_t weight;
        uint8_t data;

        memcpy(&weight, mem, sizeof(weight));
        memcpy(&data, mem + sizeof(weight), sizeof(data));

        if (leafs[data] == NULL)
        {
            leafs[data] = create_node(NULL);
            leafs[data]->weight = weight;
            leafs[data]->data = data;
        }

        mem += SIZE_LEAF_IN_FILE;
    }

    return mem - save_mem;
}

int write_header(uint8_t *out_mem, const header_t *header)
{
    const uint8_t *save_mem = out_mem;

    memcpy(out_mem, &header->padding_bits, sizeof(header->padding_bits));
    out_mem += sizeof(header->padding_bits);
    memcpy(out_mem, &header->nb_leaf, sizeof(header->nb_leaf));
    out_mem += sizeof(header->nb_leaf);

    return out_mem - save_mem;
}
