/*
** huffmantree.c for huffman in /home/leroy_v/Perso/huffman
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Tue Feb 24 20:53:40 2015 vincent leroy
** Last update Tue Feb 24 21:06:06 2015 vincent leroy
*/

#include <stdlib.h>

#include "huffmantree.h"

huffmantree_t* create_tree()
{
    huffmantree_t *tree = calloc(1, sizeof(huffmantree_t));
    return tree;
}

void delete_tree(huffmantree_t *tree)
{
    if (tree == NULL)
        return ;

    delete_node(tree->root);
    free(tree);
}
