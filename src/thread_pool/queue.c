/*
** queue.c for Huffman in /home/leroy_v/Perso/Huffman/src/thread_pool
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Wed Apr 01 15:50:28 2015 vincent leroy
** Last update Sat Apr 25 20:34:58 2015 vincent leroy
*/

#include <stdlib.h>

#include "malloc.h"
#include "queue.h"

static struct s_queuenode* create_node(void *data)
{
    struct s_queuenode *node = malloc(sizeof(struct s_queuenode));
    node->next = NULL;
    node->data = data;

    return node;
}

static void delete_node(struct s_queuenode *node)
{
    if (node == NULL)
        return ;

    free(node);
}

t_queue* create_queue()
{
    t_queue *queue = malloc(sizeof(t_queue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;

    return queue;
}

void delete_queue(t_queue *queue)
{
    if (queue == NULL)
        return ;

    struct s_queuenode *node = queue->head;
    while (node != NULL)
    {
        struct s_queuenode *next = node->next;
        delete_node(node);
        node = next;
    }

    free(queue);
}

void queue_enqueue(t_queue *queue, void *data)
{
    struct s_queuenode *node = create_node(data);

    if (queue->tail == NULL)
    {
        queue->head = node;
        queue->tail = node;
        queue->size = 1;
    }
    else
    {
        queue->tail->next = node;
        queue->tail = node;
        ++queue->size;
    }
}

void* queue_dequeue(t_queue *queue)
{
    if (queue->size == 0)
        return NULL;

    struct s_queuenode *node = queue->head;
    queue->head = node->next;
    if (queue->head == NULL)
        queue->tail = NULL;
    --queue->size;

    void *data = node->data;
    delete_node(node);
    return data;
}

void* queue_head(t_queue *queue)
{
    if (queue->size == 0)
        return NULL;

    return queue->head->data;
}

int queue_size(t_queue *queue)
{
    return queue->size;
}

int queue_is_empty(t_queue *queue)
{
    return queue->size == 0;
}

t_queueitr* queue_remove_itr(t_queue *queue, t_queueitr *itr)
{
    struct s_queuenode *prev = NULL;
    struct s_queuenode *current = queue->head;

    while (current != NULL && current != itr)
    {
        prev = current;
        current = current->next;
    }

    if (current == NULL)
        return NULL;

    if (prev != NULL)
        prev->next = current->next;
    else
        queue->head = current->next;

    if (queue->tail == current)
        queue->tail = queue->head;

    t_queueitr *next_itr = itr->next;
    delete_node(itr);
    --queue->size;
    return next_itr;
}

t_queueitr* queue_begin(t_queue *queue)
{
    return queue->head;
}

t_queueitr* queue_end(t_queue *queue)
{
    (void)queue;
    return NULL;
}

void queue_itr_incr(t_queueitr *itr)
{
    itr = itr->next;
}

void* queue_itr_data(t_queueitr *itr)
{
    return itr->data;
}
