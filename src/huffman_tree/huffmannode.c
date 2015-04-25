/*
** huffmannode.c for huffman in /home/leroy_v/Perso/Huffman/src/huffman_tree
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Feb 23 20:27:38 2015 vincent leroy
** Last update Sat Apr 25 20:28:17 2015 vincent leroy
*/

#include <stdlib.h>

#include "huffmannode.h"

t_huffmannode* create_node()
{
    t_huffmannode *node = calloc(1, sizeof(t_huffmannode));

    return node;
}

void delete_node(const t_huffmannode *node)
{
    if (node == NULL)
        return ;

    delete_node(node->left);
    delete_node(node->right);

    free((void*)node);
}
