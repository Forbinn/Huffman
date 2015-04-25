/*
** syncqueue.c for Huffman in /home/leroy_v/Perso/Huffman/src/thread_pool
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Tue Mar 31 22:46:18 2015 vincent leroy
** Last update Sat Apr 25 20:34:42 2015 vincent leroy
*/

#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#include "syncqueue.h"

t_syncqueue* create_syncqueue()
{
    t_syncqueue *queue = malloc(sizeof(t_syncqueue));

    queue->queue = create_queue();
    pthread_mutex_init(&queue->lock, NULL);
    pthread_cond_init(&queue->cond, NULL);

    return queue;
}

void delete_syncqueue(t_syncqueue *queue)
{
    if (queue == NULL)
        return ;

    pthread_mutex_lock(&queue->lock);
    delete_queue(queue->queue);
    pthread_mutex_unlock(&queue->lock);

    pthread_cond_destroy(&queue->cond);
    pthread_mutex_destroy(&queue->lock);
    free(queue);
}

void syncqueue_enqueue(t_syncqueue *queue, void *data)
{
    pthread_mutex_lock(&queue->lock);
    queue_enqueue(queue->queue, data);
    pthread_mutex_unlock(&queue->lock);

    pthread_cond_signal(&queue->cond);
}

void* syncqueue_dequeue(t_syncqueue *queue)
{
    pthread_mutex_lock(&queue->lock);
    while (queue_is_empty(queue->queue))
        pthread_cond_wait(&queue->cond, &queue->lock);

    void *data = queue_dequeue(queue->queue);
    pthread_mutex_unlock(&queue->lock);

    return data;
}

void* syncqueue_cancelable_dequeue(t_syncqueue *queue, t_thread *thread)
{
    pthread_mutex_lock(&queue->lock);
    while (queue_is_empty(queue->queue))
    {
        struct timespec t;
        clock_gettime(CLOCK_REALTIME, &t);
        t.tv_nsec += 100000000; // 100 ms
        if (t.tv_nsec >= (1000000000 - 1))
        {
            t.tv_sec += t.tv_nsec / (1000000000 - 1);
            t.tv_nsec %= (1000000000 - 1);
        }

        if (pthread_cond_timedwait(&queue->cond, &queue->lock, &t) != 0)
            if (thread_is_cancel(thread))
            {
                pthread_mutex_unlock(&queue->lock);
                return NULL;
            }
    }

    void *data = queue_dequeue(queue->queue);
    pthread_mutex_unlock(&queue->lock);

    return data;
}

int syncqueue_size(t_syncqueue *queue)
{
    pthread_mutex_lock(&queue->lock);
    int size = queue_size(queue->queue);
    pthread_mutex_unlock(&queue->lock);

    return size;
}

int syncqueue_is_empty(t_syncqueue *queue)
{
    return syncqueue_size(queue) > 0;
}
