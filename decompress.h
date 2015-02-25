/*
** decompress.h for huffman in /home/leroy_v/Perso/huffman
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Tue Feb 24 21:41:25 2015 vincent leroy
** Last update Tue Feb 24 22:12:43 2015 vincent leroy
*/

#ifndef DECOMPRESS_H_
# define DECOMPRESS_H_

#include "huffman.h"

int decompress(const char *filename, const char *out_filename);
int decompress_memory(const uint8_t *mem, int size, uint8_t *out_mem);

#endif /* !DECOMPRESS_H_ */
