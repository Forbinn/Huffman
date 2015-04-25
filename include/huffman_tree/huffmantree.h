/*
** huffmantree.h for huffman in /home/leroy_v/Perso/Huffman/include/huffman_tree
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Tue Feb 24 20:52:01 2015 vincent leroy
** Last update Sun Apr 12 12:56:12 2015 vincent leroy
*/

#ifndef HUFFMANTREE_H_
# define HUFFMANTREE_H_

#include <limits.h>

#include "huffmannode.h"

#define MAX_VALUE (UCHAR_MAX + 1)

typedef struct s_huffmantree t_huffmantree;
struct s_huffmantree
{
    t_huffmannode *root;

    uint8_t nb_leaf;
};

t_huffmantree* create_tree();
void delete_tree(const t_huffmantree *tree);
void tree_to_leafs(const t_huffmannode *node, const t_huffmannode *leafs[MAX_VALUE], uint16_t *current_idx);

#endif /* !HUFFMANTREE_H_ */
