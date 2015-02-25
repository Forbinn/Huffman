/*
** main.c for huffman in /home/leroy_v/Perso/huffman
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Feb 23 17:24:41 2015 vincent leroy
** Last update Wed Feb 25 18:08:37 2015 vincent leroy
*/

#include <stdlib.h>
#include <stdio.h>

#include "treetograph.h"
#include "predictsize.h"
#include "huffman.h"
#include "compress.h"
#include "decompress.h"

int main(int ac, char **av)
{
    if (ac != 2)
    {
        fprintf(stderr, "Usage: %s _file_\n", av[0]);
        return 1;
    }

    char name[1024];
    huffmantree_t *tree = get_tree_from_file(av[1]);
    if (tree == NULL)
        fprintf(stderr, "Unable to create the huffman tree: %m\n");
    else
    {
        printf("Huffman tree created with %d different characters\n", tree->header.nb_leaf);
        printf("predicted size = %ld bits (%ld bytes) with %d padding bits\n", tree->predict_size_bits, tree->predict_size_bytes, tree->header.padding_bits);

#if 1
        snprintf(name, 1024, "%s.hc", av[1]);
        if (compress(av[1], tree, name) == -1)
            fprintf(stderr, "Unable to compress the file: %m\n");
        else
        {
            printf("File compressed\n");

#if 1
            char decompressed_name[1024];
            snprintf(decompressed_name, 1024, "%s.hcd", name);
            if (decompress(name, decompressed_name) == -1)
                fprintf(stderr, "Unable to decompress the file: %m\n");
            else
                printf("File decompressed\n");
#endif
        }
#endif
#if 0
        snprintf(name, 1024, "%s.dot", av[1]);
        if (tree_to_graph(name, tree->root) == -1)
            fprintf(stderr, "Unable to translate the tree to a graph: %m\n");
        else
        {
            printf("Graph created\n");

            char command[1024];
            snprintf(command, 1024, "./binarytree.sh %s", name);
            system(command);
        }
#endif

        delete_tree(tree);
    }

    return 0;
}
