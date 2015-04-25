/*
** future.c for Huffman in /home/leroy_v/Perso/Huffman/src/thread_pool
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Tue Mar 31 22:34:42 2015 vincent leroy
** Last update Sat Apr 25 20:31:56 2015 vincent leroy
*/

#include <pthread.h>
#include <stdlib.h>

#include "future.h"

t_future* create_future(void* (*start_routine)(void*), void *arg)
{
    t_future *future = malloc(sizeof(t_future));
    future->start_routine = start_routine;
    future->arg = arg;
    future->result = NULL;
    future->has_finished = 0;
    pthread_mutex_init(&future->lock, NULL);
    pthread_cond_init(&future->cond, NULL);

    return future;
}

void delete_future(t_future *future)
{
    if (future == NULL)
        return ;

    pthread_mutex_destroy(&future->lock);
    pthread_cond_destroy(&future->cond);
    free(future);
}

void* future_get_result(t_future *future)
{
    pthread_mutex_lock(&future->lock);
    while (future->has_finished == 0)
        pthread_cond_wait(&future->cond, &future->lock);

    void *result = future->result;
    pthread_mutex_unlock(&future->lock);

    return result;
}

void future_set_finished(t_future *future)
{
    pthread_mutex_lock(&future->lock);
    future->has_finished = 1;
    pthread_mutex_unlock(&future->lock);

    pthread_cond_signal(&future->cond);
}

int future_has_finished(t_future *future)
{
    pthread_mutex_lock(&future->lock);
    int has_finished = future->has_finished;
    pthread_mutex_unlock(&future->lock);

    return has_finished;
}

void future_wait_for_finish(t_future *future)
{
    pthread_mutex_lock(&future->lock);
    while (future->has_finished == 0)
        pthread_cond_wait(&future->cond, &future->lock);
    pthread_mutex_unlock(&future->lock);
}
