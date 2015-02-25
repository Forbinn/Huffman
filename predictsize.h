/*
** predictsize.h for huffman in /home/leroy_v/Perso/huffman
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Tue Feb 24 16:55:04 2015 vincent leroy
** Last update Tue Feb 24 22:19:28 2015 vincent leroy
*/

#ifndef PREDICTSIZE_H_
# define PREDICTSIZE_H_

#include "huffmannode.h"

uint64_t predict_size_bit(const huffmannode_t *root);
uint64_t predict_size_byte(const huffmannode_t *root);

uint64_t predict_size_from_memory_bit(const uint8_t *header);
uint64_t predict_size_from_memory_byte(const uint8_t *header);

#endif /* !PREDICTSIZE_H_ */
