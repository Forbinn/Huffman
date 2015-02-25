/*
** header.h for huffman in /home/leroy_v/Perso/huffman
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Wed Feb 25 17:33:34 2015 vincent leroy
** Last update Wed Feb 25 18:02:26 2015 vincent leroy
*/

#ifndef HEADER_H_
# define HEADER_H_

#include "huffmannode.h"

#define MEMBER_SIZE(type, member)   sizeof(((type*)0)->member)

#define MAX_VALUE                   256
#define SIZE_LEAF_IN_FILE           (MEMBER_SIZE(huffmannode_t, weight) + MEMBER_SIZE(huffmannode_t, data))

typedef struct header_s header_t;
struct header_s
{
    uint8_t padding_bits;
    uint16_t nb_leaf;
};

uint32_t get_header_size();
int read_header(const uint8_t *mem, header_t *header, huffmannode_t *leafs[MAX_VALUE]);
int write_header(uint8_t *out_mem, const header_t *header);

#endif /* !HEADER_H_ */
