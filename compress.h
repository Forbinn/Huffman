/*
** compress.h for huffman in /home/leroy_v/Perso/huffman
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Tue Feb 24 17:23:42 2015 vincent leroy
** Last update Tue Feb 24 21:42:21 2015 vincent leroy
*/

#ifndef COMPRESS_H_
# define COMPRESS_H_

#include "huffmantree.h"

typedef struct byte_to_bit_s byte_to_bit_t;
struct byte_to_bit_s
{
    uint32_t bits;
    int nb_bits;
};

int compress(const char *filename, const huffmantree_t *tree, const char *out_filename);
int compress_memory(const uint8_t *mem, int size, const huffmantree_t *tree, uint8_t *out_mem);

#endif /* !COMPRESS_H_ */
