/*
** huffmannode.h for huffman in /home/leroy_v/Perso/huffman
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Feb 23 19:12:58 2015 vincent leroy
** Last update Wed Feb 25 17:43:30 2015 vincent leroy
*/

#ifndef HUFFMANNODE_H_
# define HUFFMANNODE_H_

#include <stdint.h>

typedef struct huffmannode_s huffmannode_t;
struct huffmannode_s
{
    huffmannode_t *parent;
    huffmannode_t *left;
    huffmannode_t *right;

    uint32_t weight;
    uint8_t data;
};

huffmannode_t* create_node(huffmannode_t *parent);
void delete_node(huffmannode_t *node);

#endif /* !HUFFMANNODE_H_ */
