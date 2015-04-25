/*
** pool.h for Huffman in /home/leroy_v/Perso/Huffman/thread_pool
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Tue Mar 31 22:23:27 2015 vincent leroy
** Last update Tue Apr 07 17:17:41 2015 vincent leroy
*/

#ifndef POOL_H_
# define POOL_H_

#include <sys/types.h>

#include "thread.h"
#include "future.h"
#include "syncqueue.h"

typedef struct s_pool
{
    t_thread **threads;

    pthread_mutex_t lock;
    int nb_max_thread;
    int current_thread_count;
    int nb_running_thread;

    t_syncqueue *queue;
} t_pool;

t_pool* create_pool(int nb_max_thread);
void delete_pool(t_pool *pool);

t_future* pool_submit_task(t_pool *pool, void* (*start_routine)(void*), void *arg);

#endif /* !POOL_H_ */
