/*
** pool.c for Huffman in /home/leroy_v/Perso/Huffman/src/thread_pool
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Tue Mar 31 22:23:27 2015 vincent leroy
** Last update Sat Apr 25 20:32:22 2015 vincent leroy
*/

#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

#include "pool.h"

struct s_thread_arg
{
    t_pool *pool;
    t_thread *current;
};

static void* routine(void *arg)
{
    struct s_thread_arg *thread_arg = (struct s_thread_arg*)arg;
    t_pool *pool = thread_arg->pool;

    while (!thread_is_cancel(thread_arg->current))
    {
        t_future *future = (t_future*)syncqueue_cancelable_dequeue(pool->queue, thread_arg->current);
        if (future == NULL)
            break;

        pthread_mutex_lock(&pool->lock);
        ++pool->nb_running_thread;
        pthread_mutex_unlock(&pool->lock);

        future->result = (*future->start_routine)(future->arg);

        pthread_mutex_lock(&pool->lock);
        --pool->nb_running_thread;
        pthread_mutex_unlock(&pool->lock);

        future_set_finished(future);
    }

    free(thread_arg);
    pthread_exit(NULL);
}

t_pool* create_pool(int nb_max_thread)
{
    if (nb_max_thread < 1)
    {
        errno = EINVAL;
        return NULL;
    }

    int i;
    t_pool *pool = malloc(sizeof(t_pool));

    pool->threads = malloc(nb_max_thread * sizeof(t_thread*));
    for (i = 0; i < nb_max_thread; ++i)
        pool->threads[i] = create_thread();

    pthread_mutex_init(&pool->lock, NULL);
    pool->nb_max_thread = nb_max_thread;
    pool->current_thread_count = 0;
    pool->nb_running_thread = 0;
    pool->queue = create_syncqueue();

    return pool;
}

void delete_pool(t_pool *pool)
{
    int i;

    if (pool == NULL)
        return ;

    for (i = 0; i < pool->nb_max_thread; ++i)
        delete_thread(pool->threads[i]);

    pthread_mutex_destroy(&pool->lock);
    delete_syncqueue(pool->queue);
    free(pool->threads);
    free(pool);
}

t_future* pool_submit_task(t_pool *pool, void* (*start_routine)(void*), void *arg)
{
    t_future *future = create_future(start_routine, arg);
    syncqueue_enqueue(pool->queue, future);

    pthread_mutex_lock(&pool->lock);
    if (pool->nb_running_thread == pool->current_thread_count && pool->current_thread_count < pool->nb_max_thread)
    {
        struct s_thread_arg *arg = malloc(sizeof(struct s_thread_arg));
        arg->pool = pool;
        arg->current = pool->threads[pool->current_thread_count];

        thread_start(pool->threads[pool->current_thread_count], &routine, arg);
        ++pool->current_thread_count;
    }
    pthread_mutex_unlock(&pool->lock);

    return future;
}
