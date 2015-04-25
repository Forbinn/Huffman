/*
** thread.h for Huffman in /home/leroy_v/Perso/Huffman/thread_pool
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Wed Apr 01 15:06:17 2015 vincent leroy
** Last update Tue Apr 07 17:17:55 2015 vincent leroy
*/

#ifndef THREAD_H_
# define THREAD_H_

#include <sys/types.h>

typedef struct s_thread
{
    pthread_t id;

    pthread_mutex_t lock;
    int run;
    int cancel;
} t_thread;

int number_of_cores();

t_thread* create_thread();
void delete_thread(t_thread *thread);

void thread_start(t_thread *thread, void* (*start_routine)(void*), void *arg);
void thread_cancel(t_thread *thread);
void* thread_cancel_and_wait(t_thread *thread);
int thread_is_running(t_thread *thread);
int thread_is_cancel(t_thread *thread);

#endif /* !THREAD_H_ */
