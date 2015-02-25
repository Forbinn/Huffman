/*
** treetograph.h for huffman in /home/leroy_v/Perso/huffman
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Feb 23 20:14:54 2015 vincent leroy
** Last update Mon Feb 23 23:27:17 2015 vincent leroy
*/

#ifndef TREETOGRAPH_H_
# define TREETOGRAPH_H_

#include "huffmannode.h"

int tree_to_graph(const char *filename, const huffmannode_t *root);

#endif /* !TREETOGRAPH_H_ */
