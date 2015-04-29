/*
** huffman.c for huffman in /home/leroy_v/Perso/Huffman/src/huffman_tree
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Feb 23 23:25:05 2015 vincent leroy
** Last update Wed Apr 29 13:44:13 2015 vincent leroy
*/

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "huffman.h"

static void sort_queue(t_huffmannode *queue[MAX_VALUE])
{
    int i;

    for (i = 0; i < MAX_VALUE; ++i)
    {
        int idx = -1;
        int j;

        for (j = i; j < MAX_VALUE; ++j)
        {
            if (queue[j] == NULL)
                continue;

            if (idx < 0 || queue[j]->weight < queue[idx]->weight)
                idx = j;
        }

        if (idx == -1)
            break;
        else if (idx == i)
            continue;

        t_huffmannode *tmp = queue[i];
        queue[i] = queue[idx];
        queue[idx] = tmp;
    }
}

static void move_tab(t_huffmannode *tab[MAX_VALUE])
{
    memmove(tab, tab + 1, (MAX_VALUE - 1) * sizeof(t_huffmannode*));
    tab[MAX_VALUE - 1] = NULL;
}

static t_huffmannode* get_min_weight_node(t_huffmannode *queue1[MAX_VALUE], t_huffmannode *queue2[MAX_VALUE])
{
    t_huffmannode *node;

    if (queue1[0] == NULL)
    {
        node = queue2[0];
        move_tab(queue2);
    }
    else if (queue2[0] == NULL || queue1[0]->weight <= queue2[0]->weight)
    {
        node = queue1[0];
        move_tab(queue1);
    }
    else
    {
        node = queue2[0];
        move_tab(queue2);
    }

    return node;
}

static t_huffmannode* compute_huffman_tree(t_huffmannode *queue1[MAX_VALUE])
{
    t_huffmannode *queue2[MAX_VALUE] = {};

    while (!((queue1[0] == NULL && queue2[1] == NULL) || (queue1[1] == NULL && queue2[0] == NULL)))
    {
        t_huffmannode *node1 = get_min_weight_node(queue1, queue2);
        t_huffmannode *node2 = get_min_weight_node(queue1, queue2);

        t_huffmannode *internal_node = create_node(NULL);
        internal_node->left = node1->weight < node2->weight ? node1 : node2;
        internal_node->right = internal_node->left == node1 ? node2 : node1;
        internal_node->weight = node1->weight + node2->weight;

        int i;
        for (i = 0; i < MAX_VALUE; ++i)
            if (queue2[i] == NULL)
            {
                queue2[i] = internal_node;
                break;
            }
    }

    return queue1[0] != NULL ? queue1[0] : queue2[0];
}

static void process_block_size(const uint8_t *mem, uint64_t size, t_huffmannode *leafs[MAX_VALUE])
{
    uint64_t i;

    for (i = 0; i < size; ++i)
    {
        t_huffmannode *node = leafs[mem[i]];
        if (node == NULL)
        {
            node = create_node();
            node->data = mem[i];
            leafs[mem[i]] = node;
        }

        ++node->weight;
    }
}

t_huffmantree* get_tree_from_file_read(int fd, uint64_t block_size)
{
    uint8_t *mem;
    int64_t size_read = -1;
    t_huffmannode *queue[MAX_VALUE] = {};

    mem = malloc(block_size * sizeof(uint8_t));
    while ((size_read = read(fd, mem, block_size * sizeof(uint8_t))) > 0)
        process_block_size(mem, size_read, queue);
    free(mem);

    if (size_read == -1)
        return NULL;

    return get_tree_from_leaf(queue);
}

t_huffmantree* get_tree_from_memory(const uint8_t *mem, uint64_t size)
{
    t_huffmannode *queue[MAX_VALUE] = {};

    process_block_size(mem, size, queue);
    return get_tree_from_leaf(queue);
}

t_huffmantree* get_tree_from_leaf(t_huffmannode *leafs[MAX_VALUE])
{
    t_huffmantree *tree;
    t_huffmannode *root;
    uint16_t nb_leaf;

    sort_queue(leafs);
    for (nb_leaf = 0; nb_leaf < MAX_VALUE && leafs[nb_leaf] != NULL; ++nb_leaf);

    root = compute_huffman_tree(leafs);
    if (root == NULL)
        return NULL;
    else if (root->left == NULL && root->right == NULL) // only 1 character in the file
    {
        t_huffmannode *node = create_node(NULL);
        node->left = root;
        root = node;
    }

    tree = create_tree();
    tree->root = root;
    tree->nb_leaf = nb_leaf - 1;
    return tree;
}
