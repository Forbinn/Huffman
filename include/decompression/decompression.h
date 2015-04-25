/*
** decompression.h for huffman in /home/leroy_v/Perso/Huffman/src/decompression
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Tue Feb 24 21:41:25 2015 vincent leroy
** Last update Tue Apr 07 17:14:43 2015 vincent leroy
*/

#ifndef UNCOMPRESSION_H_
# define UNCOMPRESSION_H_

#include "huffman.h"

int decode_byte(const t_huffmantree *tree, const uint8_t *mem, int off, uint8_t *byte);

#endif /* !UNCOMPRESSION_H_ */
