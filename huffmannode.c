/*
** huffmannode.c for huffman in /home/leroy_v/Perso/huffman
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Feb 23 20:27:38 2015 vincent leroy
** Last update Mon Feb 23 20:40:45 2015 vincent leroy
*/

#include <stdlib.h>

#include "huffmannode.h"

huffmannode_t* create_node(huffmannode_t *parent)
{
    huffmannode_t *node = calloc(1, sizeof(huffmannode_t));
    node->parent = parent;

    return node;
}

void delete_node(huffmannode_t *node)
{
    if (node == NULL)
        return ;

    delete_node(node->left);
    delete_node(node->right);

    free(node);
}
