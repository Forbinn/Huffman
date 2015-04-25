/*
** compression.h for huffman in /home/leroy_v/Perso/Huffman/src/compression
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Tue Feb 24 17:23:42 2015 vincent leroy
** Last update Tue Apr 07 17:15:59 2015 vincent leroy
*/

#ifndef COMPRESSION_H_
# define COMPRESSION_H_

#include "huffman.h"

typedef struct s_byte_to_bit
{
    uint32_t bits;
    int nb_bits;
} t_byte_to_bit;

void tree_to_tab(const t_huffmantree *tree, t_byte_to_bit tab[MAX_VALUE]);
void write_code(uint8_t *out_mem, int off, const t_byte_to_bit *code);

#endif /* !COMPRESSION_H_ */
