/*
** syncqueue.h for Huffman in /home/leroy_v/Perso/Huffman/thread_pool
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Tue Mar 31 22:46:22 2015 vincent leroy
** Last update Tue Apr 07 17:17:49 2015 vincent leroy
*/

#ifndef SYNCQUEUE_H_
# define SYNCQUEUE_H_

#include <sys/types.h>

#include "queue.h"
#include "thread.h"

typedef struct s_syncqueue
{
    t_queue *queue;

    pthread_mutex_t lock;
    pthread_cond_t cond;
} t_syncqueue;

t_syncqueue* create_syncqueue();
void delete_syncqueue(t_syncqueue *queue);

void syncqueue_enqueue(t_syncqueue *queue, void *data);
void* syncqueue_dequeue(t_syncqueue *queue);
void* syncqueue_cancelable_dequeue(t_syncqueue *queue, t_thread *thread);
int synqueue_size(t_syncqueue *queue);
int syncqueue_is_empty(t_syncqueue *queue);

#endif /* !SYNCQUEUE_H_ */
