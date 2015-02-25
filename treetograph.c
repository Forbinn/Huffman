/*
** treetograph.c for huffman in /home/leroy_v/Perso/huffman
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Feb 23 20:14:59 2015 vincent leroy
** Last update Wed Feb 25 16:42:54 2015 vincent leroy
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>

#include "treetograph.h"

static void write_beginning(int fd)
{
    dprintf(fd, "digraph huffman {\n");
    dprintf(fd, "\tgraph [ordering=\"out\"];\n");
}

static void write_ending(int fd)
{
    dprintf(fd, "}\n");
}

static void write_single_node(int fd, const huffmannode_t *node)
{
    if (node == NULL)
        return ;

    if (node->left != NULL || node->right != NULL)
        dprintf(fd, "\"%d\n%p\"", node->weight, node);
    else if (isalnum(node->data))
        dprintf(fd, "\"%c\n%d\"", node->data, node->weight);
    else
        dprintf(fd, "\"0x%02x\n%d\"", node->data, node->weight);
}

static void write_node(int fd, const huffmannode_t *node)
{
    if (node == NULL || (node->left == NULL && node->right == NULL))
        return ;

    if (node->left != NULL)
    {
        dprintf(fd, "\t");
        write_single_node(fd, node);
        dprintf(fd, " -> ");
        write_single_node(fd, node->left);
        dprintf(fd, ";\n");
    }
    if (node->right != NULL)
    {
        dprintf(fd, "\t");
        write_single_node(fd, node);
        dprintf(fd, " -> ");
        write_single_node(fd, node->right);
        dprintf(fd, ";\n");
    }

    write_node(fd, node->left);
    write_node(fd, node->right);
}

int tree_to_graph(const char *filename, const huffmannode_t *root)
{
    if (root == NULL)
        return -1;

    int fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd == -1)
        return -1;
    write_beginning(fd);

    write_node(fd, root);

    write_ending(fd);
    close(fd);
    return 0;
}
