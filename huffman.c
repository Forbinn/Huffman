/*
** huffman.c for huffman in /home/leroy_v/Perso/huffman
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Feb 23 23:25:05 2015 vincent leroy
** Last update Wed Feb 25 17:58:40 2015 vincent leroy
*/

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "predictsize.h"
#include "huffman.h"

static void sort_queue(huffmannode_t *queue[MAX_VALUE])
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

        huffmannode_t *tmp = queue[i];
        queue[i] = queue[idx];
        queue[idx] = tmp;
    }
}

static void move_tab(huffmannode_t *tab[MAX_VALUE])
{
    memmove(tab, tab + 1, (MAX_VALUE - 1) * sizeof(huffmannode_t*));
    tab[MAX_VALUE - 1] = NULL;
}

static huffmannode_t* get_min_weight_node(huffmannode_t *queue1[MAX_VALUE], huffmannode_t *queue2[MAX_VALUE])
{
    huffmannode_t *node;

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

static huffmannode_t* compute_huffman_tree(huffmannode_t *queue1[MAX_VALUE])
{
    huffmannode_t *queue2[MAX_VALUE] = {};

    while (!((queue1[0] == NULL && queue2[1] == NULL) || (queue1[1] == NULL && queue2[0] == NULL)))
    {
        huffmannode_t *node1 = get_min_weight_node(queue1, queue2);
        huffmannode_t *node2 = get_min_weight_node(queue1, queue2);

        huffmannode_t *internal_node = create_node(NULL);
        internal_node->left = node1->weight < node2->weight ? node1 : node2;
        internal_node->right = internal_node->left == node1 ? node2 : node1;
        internal_node->weight = node1->weight + node2->weight;
        node1->parent = internal_node;
        node2->parent = internal_node;

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

huffmantree_t* get_tree_from_file(const char *filename)
{
    struct stat st;
    int fd;
    uint8_t *mem;
    huffmantree_t *tree = NULL;

    if (stat(filename, &st) != -1)
    {
        fd = open(filename, O_RDONLY);
        if (fd != -1)
        {
            mem = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
            if (mem != MAP_FAILED)
            {
                tree = get_tree_from_memory(mem, st.st_size);
                munmap(mem, st.st_size);
            }
            close(fd);
        }
    }

    return tree;
}

huffmantree_t* get_tree_from_memory(const uint8_t *mem, int size)
{
    huffmannode_t *queue[MAX_VALUE] = {};
    int i;

    for (i = 0; i < size; ++i)
    {
        huffmannode_t *node = queue[mem[i]];
        if (node == NULL)
        {
            node = create_node(NULL);
            node->data = mem[i];
        }

        ++node->weight;
        queue[mem[i]] = node;
    }

    return get_tree_from_leaf(queue);
}

huffmantree_t* get_tree_from_leaf(huffmannode_t *leafs[MAX_VALUE])
{
    huffmantree_t *tree;
    huffmannode_t *root;
    uint16_t nb_leaf;

    sort_queue(leafs);
    for (nb_leaf = 0; leafs[nb_leaf] != NULL && nb_leaf < MAX_VALUE; ++nb_leaf);

    root = compute_huffman_tree(leafs);
    if (root == NULL)
        return NULL;

    tree = create_tree();
    tree->root = root;
    tree->predict_size_bits = predict_size_bit(tree->root);
    tree->predict_size_bytes = tree->predict_size_bits / 8 + ((tree->predict_size_bits % 8) != 0);
    tree->header.padding_bits = tree->predict_size_bytes * 8 - tree->predict_size_bits;
    tree->header.nb_leaf = nb_leaf;
    return tree;
}
