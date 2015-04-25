/*
** huffmannode.h for huffman in /home/leroy_v/Perso/Huffman/include/huffman_tree
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Feb 23 19:12:58 2015 vincent leroy
** Last update Sat Apr 11 18:42:56 2015 vincent leroy
*/

#ifndef HUFFMANNODE_H_
# define HUFFMANNODE_H_

#include <stdint.h>

typedef struct s_huffmannode t_huffmannode;
struct s_huffmannode
{
    t_huffmannode *left;
    t_huffmannode *right;

    uint32_t weight;
    uint8_t data;
};

t_huffmannode* create_node();
void delete_node(const t_huffmannode *node);

#endif /* !HUFFMANNODE_H_ */
