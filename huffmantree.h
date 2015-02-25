/*
** huffmantree.h for huffman in /home/leroy_v/Perso/huffman
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Tue Feb 24 20:52:01 2015 vincent leroy
** Last update Wed Feb 25 17:53:18 2015 vincent leroy
*/

#ifndef HUFFMANTREE_H_
# define HUFFMANTREE_H_

#include "header.h"
#include "huffmannode.h"

typedef struct huffmantree_s huffmantree_t;
struct huffmantree_s
{
    header_t header;

    huffmannode_t *root;

    uint64_t predict_size_bits;
    uint64_t predict_size_bytes;
};

huffmantree_t* create_tree();
void delete_tree(huffmantree_t *tree);

#endif /* !HUFFMANTREE_H_ */
