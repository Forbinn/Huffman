/*
** decompression_threaded.c for decompression in /home/leroy_v/Perso/Huffman/src/decompression
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Sun Apr 05 02:02:05 2015 vincent leroy
** Last update Sat Apr 25 21:02:06 2015 vincent leroy
*/

#include <stdio.h>
#include <stdlib.h>

#include "pool.h"
#include "queue.h"
#include "crc32.h"
#include "decompression.h"
#include "decompression_threaded.h"

#define max(x, y)   ((x) > (y) ? (x) : (y))

static uint32_t compute_crc32_decompression(const t_decompress_arg *arg)
{
    uint32_t i;
    uint32_t crc = 0;
    uint16_t nb_leaf = 0;
    const t_huffmannode *leafs[MAX_VALUE] = {};
    tree_to_leafs(arg->tree->root, leafs, &nb_leaf);

    crc = crc32(crc, &arg->tree->nb_leaf, sizeof(arg->tree->nb_leaf));
    for (i = 0; i < nb_leaf; ++i)
    {
        crc = crc32(crc, &leafs[i]->weight, sizeof(leafs[i]->weight));
        crc = crc32(crc, &leafs[i]->data, sizeof(leafs[i]->data));
    }

    crc = crc32(crc, &arg->padding, sizeof(arg->padding));
    crc = crc32(crc, &arg->mem_size, sizeof(arg->mem_size));
    return crc32(crc, arg->mem, arg->mem_size);
}

static void* decompress_thread(void *arg)
{
    t_decompress_arg *decompress_arg = (t_decompress_arg*)arg;
    uint32_t crc = compute_crc32_decompression(decompress_arg);
    if (crc != decompress_arg->crc)
        return NULL;

    t_decompress_ret *decompress_ret = calloc(1, sizeof(t_decompress_ret));
    decompress_ret->mem = malloc(decompress_arg->block_size * sizeof(uint8_t));

    uint64_t i = 0;
    uint32_t counter = 0;
    int off = 0;
    uint64_t mem_size_bits = (decompress_arg->mem_size * 8) - decompress_arg->padding;
    while (i < mem_size_bits)
    {
        i += decode_byte(decompress_arg->tree, decompress_arg->mem + (i / 8), off, decompress_ret->mem + counter);
        off = i % 8;
        ++counter;
    }

    decompress_ret->mem_size = counter;

    return decompress_ret;
}

static int process_future(t_future *future, d_output out, void *out_userdata)
{
    int ret = 0;

    t_decompress_ret *decompress_ret = (t_decompress_ret*)future_get_result(future);
    t_decompress_arg *decompress_arg = (t_decompress_arg*)future->arg;
    delete_tree(decompress_arg->tree);
    free(decompress_arg->mem);
    free(decompress_arg);

    if (decompress_ret != NULL)
    {
        uint32_t size_write = (*out)(decompress_ret, out_userdata);
        if (size_write == (uint32_t)-1)
            ret = -2;

        free(decompress_ret->mem);
        free(decompress_ret);
    }
    else // crc computation failed
        ret = -1;

    delete_future(future);
    return ret;
}

t_d_error decompress_with_thread(const t_u_thread_param *param, d_input in, d_output out, void *in_userdata, void *out_userdata)
{
    t_d_error ret = D_NO_ERROR;
    int size_queue_max = max(1, param->memory_max / ((uint64_t)param->block_size * 2));
    t_queue *queue = create_queue();
    t_pool *pool = create_pool(param->number_of_thread);

    while (ret == D_NO_ERROR)
    {
        if (!queue_is_empty(queue))
        {
            t_future *head_future = (t_future*)queue_head(queue);
            if (queue_size(queue) >= size_queue_max)
                future_wait_for_finish(head_future);

            if (future_has_finished(head_future))
            {
                int tmp = process_future(head_future, out, out_userdata);
                if (tmp == -1)
                    ret = D_CRC_ERROR;
                else if (tmp == -2)
                    ret = D_WRITE_ERROR;
                queue_dequeue(queue);
            }

            if (ret != D_NO_ERROR)
                break;
        }

        t_decompress_arg *decompress_arg = malloc(sizeof(t_decompress_arg));
        int32_t ret_in = (*in)(decompress_arg, in_userdata);
        if (ret_in <= 0)
        {
            free(decompress_arg);
            ret = ret_in == 0 ? D_NO_ERROR : D_READ_ERROR;
            break;
        }

        decompress_arg->block_size = param->block_size;
        t_future *future = pool_submit_task(pool, &decompress_thread, decompress_arg);
        queue_enqueue(queue, future);
    }

    if (ret != D_NO_ERROR)
        delete_pool(pool);

    while (!queue_is_empty(queue))
    {
        t_future *future = (t_future*)queue_dequeue(queue);
        if (ret == D_NO_ERROR)
        {
            int tmp = process_future(future, out, out_userdata);
            if (tmp == -1)
                ret = D_CRC_ERROR;
            else if (tmp == -2)
                ret = D_WRITE_ERROR;
        }
        else
        {
            t_decompress_arg *decompress_arg = (t_decompress_arg*)future->arg;
            delete_tree(decompress_arg->tree);
            free(decompress_arg->mem);
            free(decompress_arg);

            t_decompress_ret *decompress_ret = (t_decompress_ret*)future->result;
            if (decompress_ret != NULL)
            {
                free(decompress_ret->mem);
                free(decompress_ret);
            }

            delete_future(future);
        }
    }

    if (ret == D_NO_ERROR)
        delete_pool(pool);
    delete_queue(queue);
    return ret;
}

#undef max
