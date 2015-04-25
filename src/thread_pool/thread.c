/*
** thread.c for Huffman in /home/leroy_v/Perso/Huffman/src/thread_pool
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Wed Apr 01 15:06:15 2015 vincent leroy
** Last update Sat Apr 25 20:33:12 2015 vincent leroy
*/

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "thread.h"

int number_of_cores()
{
    static int cores = 0;

    if (cores == 0)
        cores = sysconf(_SC_NPROCESSORS_ONLN);

    return cores;
}

t_thread* create_thread()
{
    t_thread *thread = malloc(sizeof(t_thread));
    thread->id = 0;
    thread->run = 0;
    thread->cancel = 0;

    pthread_mutex_init(&thread->lock, NULL);

    return thread;
}

void delete_thread(t_thread *thread)
{
    if (thread == NULL)
        return ;

    if (thread->run)
        thread_cancel_and_wait(thread);

    pthread_mutex_destroy(&thread->lock);
    free(thread);
}

void thread_start(t_thread *thread, void* (*start_routine)(void*), void *arg)
{
    pthread_mutex_lock(&thread->lock);
    if (!thread->run)
    {
        thread->cancel = 0;
        thread->run = pthread_create(&thread->id, NULL, start_routine, arg) == 0;
    }
    pthread_mutex_unlock(&thread->lock);
}

void thread_cancel(t_thread *thread)
{
    pthread_mutex_lock(&thread->lock);
    thread->cancel = 1;
    pthread_mutex_unlock(&thread->lock);
}

void* thread_cancel_and_wait(t_thread *thread)
{
    void *ret_val;

    if (!thread_is_running(thread))
        return NULL;

    thread_cancel(thread);
    pthread_join(thread->id, &ret_val);

    pthread_mutex_lock(&thread->lock);
    thread->run = 0;
    pthread_mutex_unlock(&thread->lock);

    return ret_val;
}

int thread_is_running(t_thread *thread)
{
    pthread_mutex_lock(&thread->lock);
    int run = thread->run;
    pthread_mutex_unlock(&thread->lock);

    return run;
}

int thread_is_cancel(t_thread *thread)
{
    pthread_mutex_lock(&thread->lock);
    int cancel = thread->cancel;
    pthread_mutex_unlock(&thread->lock);

    return cancel;
}
