/*
** huffman.h for huffman in /home/leroy_v/Perso/Huffman/include/huffman_tree
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Feb 23 23:25:01 2015 vincent leroy
** Last update Sun Apr 12 12:56:12 2015 vincent leroy
*/

#ifndef HUFFMAN_H_
# define HUFFMAN_H_

#include "huffmantree.h"

t_huffmantree* get_tree_from_file_read(int fd, uint64_t block_size);
t_huffmantree* get_tree_from_file_mmap(int fd);
t_huffmantree* get_tree_from_memory(const uint8_t *mem, uint64_t size);
t_huffmantree* get_tree_from_leaf(t_huffmannode *leafs[MAX_VALUE]);

#endif /* !HUFFMAN_H_ */
