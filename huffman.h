/*
** huffman.h for huffman in /home/leroy_v/Perso/huffman
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Feb 23 23:25:01 2015 vincent leroy
** Last update Wed Feb 25 17:42:43 2015 vincent leroy
*/

#ifndef HUFFMAN_H_
# define HUFFMAN_H_

#include "huffmantree.h"
#include "header.h"

huffmantree_t* get_tree_from_file(const char *filename);
huffmantree_t* get_tree_from_memory(const uint8_t *mem, int size);
huffmantree_t* get_tree_from_leaf(huffmannode_t *leafs[MAX_VALUE]);

#endif /* !HUFFMAN_H_ */
