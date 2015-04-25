/*
** compression_threaded.c for compression in /home/leroy_v/Perso/Huffman/src/compression
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Fri Apr 03 16:24:16 2015 vincent leroy
** Last update Sat Apr 25 21:04:42 2015 vincent leroy
*/

#include <stdlib.h>

#include "pool.h"
#include "queue.h"
#include "crc32.h"
#include "compression.h"
#include "compression_threaded.h"

static uint32_t compute_crc32_compression(const t_compress_ret *ret)
{
    uint32_t i;
    uint32_t crc = 0;
    uint16_t nb_leaf = 0;
    const t_huffmannode *leafs[MAX_VALUE] = {};
    tree_to_leafs(ret->tree->root, leafs, &nb_leaf);

    crc = crc32(crc, &ret->tree->nb_leaf, sizeof(ret->tree->nb_leaf));
    for (i = 0; i < nb_leaf; ++i)
    {
        crc = crc32(crc, &leafs[i]->weight, sizeof(leafs[i]->weight));
        crc = crc32(crc, &leafs[i]->data, sizeof(leafs[i]->data));
    }

    crc = crc32(crc, &ret->padding, sizeof(ret->padding));
    crc = crc32(crc, &ret->mem_size, sizeof(ret->mem_size));
    return crc32(crc, ret->mem, ret->mem_size);
}

static void* compress_thread(void *arg)
{
    uint32_t i;
    uint64_t counter = 0;
    t_byte_to_bit tab[MAX_VALUE] = {};

    t_compress_arg *compress_arg = (t_compress_arg*)arg;
    const t_huffmantree *tree = get_tree_from_memory(compress_arg->mem, compress_arg->mem_size);
    t_compress_ret *compress_ret = calloc(1, sizeof(t_compress_ret));

    compress_ret->mem = calloc(compress_arg->mem_size, sizeof(uint8_t));
    compress_ret->tree = tree;
    tree_to_tab(tree, tab);

    for (i = 0; i < compress_arg->mem_size; ++i)
    {
        const t_byte_to_bit *current = &tab[compress_arg->mem[i]];
        write_code(compress_ret->mem + (counter / 8), counter % 8, current);
        counter += current->nb_bits;
    }

    compress_ret->padding = 8 - (counter % 8);
    compress_ret->mem_size = (counter / 8) + (compress_ret->padding != 0 ? 1 : 0);
    compress_ret->crc = compute_crc32_compression(compress_ret);
    return compress_ret;
}

static int process_future(t_future *future, c_output out, void *out_userdata)
{
    int ret = 0;

    t_compress_ret *compress_ret = (t_compress_ret*)future_get_result(future);
    t_compress_arg *compress_arg = (t_compress_arg*)future->arg;
    free(compress_arg->mem);
    free(compress_arg);

    uint32_t size_write = (*out)(compress_ret, out_userdata);
    if (size_write == (uint32_t)-1)
        ret = -1;

    free(compress_ret->mem);
    delete_tree(compress_ret->tree);
    free(compress_ret);
    delete_future(future);
    return ret;
}

t_c_error compress_with_thread(const t_c_thread_param *param, c_input in, c_output out, void *in_userdata, void *out_userdata)
{
    t_c_error ret = C_NO_ERROR;
    int size_queue_max = param->memory_max / ((uint64_t)param->block_size * 2);
    t_queue *queue = create_queue();
    t_pool *pool = create_pool(param->number_of_thread);

    while (ret == C_NO_ERROR)
    {
        if (!queue_is_empty(queue))
        {
            t_future *head_future = (t_future*)queue_head(queue);
            if (queue_size(queue) >= size_queue_max)
                future_wait_for_finish(head_future);

            if (future_has_finished(head_future))
            {
                if (process_future(head_future, out, out_userdata) == -1)
                    ret = C_WRITE_ERROR;
                queue_dequeue(queue);
            }

            if (ret != C_NO_ERROR)
                break;
        }

        t_compress_arg *compress_arg = malloc(sizeof(t_compress_arg));
        compress_arg->mem = malloc(param->block_size * sizeof(uint8_t));
        compress_arg->mem_size = param->block_size;

        uint32_t size_read = (*in)(compress_arg, in_userdata);
        if (size_read == 0 || size_read == (uint32_t)-1)
        {
            ret = size_read == 0 ? C_NO_ERROR : C_READ_ERROR;
            free(compress_arg->mem);
            free(compress_arg);
            break;
        }


        t_future *future = pool_submit_task(pool, &compress_thread, compress_arg);
        queue_enqueue(queue, future);
    }

    if (ret != C_NO_ERROR)
        delete_pool(pool);

    while (!queue_is_empty(queue))
    {
        t_future *future = (t_future*)queue_dequeue(queue);
        if (ret == C_NO_ERROR)
            process_future(future, out, out_userdata);
        else
        {
            t_compress_arg *compress_arg = (t_compress_arg*)future->arg;
            free(compress_arg->mem);
            free(compress_arg);

            t_compress_ret *compress_ret = (t_compress_ret*)future->result;
            if (compress_ret != NULL)
            {
                free(compress_ret->mem);
                delete_tree(compress_ret->tree);
                free(compress_ret);
            }

            delete_future(future);
        }
    }

    if (ret == C_NO_ERROR)
        delete_pool(pool);
    delete_queue(queue);
    return ret;
}
