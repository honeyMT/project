#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "threadpool.h"

// 初始化线程池
void threadpool_init(threadpool_t *pool, int rheads)
{
    condition_init(&pool->ready);
    pool->first       = NULL;
    pool->last        = NULL;
    pool->counter     = 0;
    pool->idle        = 0;
    pool->max_threads = rheads;
    pool->quit        = 0;
}

void *route(void *arg) {
    threadpool_t *pool = (threadpool_t*)arg;
    int timeout = 0;
    while ( 1 ) {
        timeout = 0;
        pool->idle++;
        condition_lock(&pool->ready);
        // 任务队列没有任务，并且没有得到退出通知，就等待
        while ( pool->first == NULL && pool->quit == 0)  {
            printf("%lu thread is waiting...\n", pthread_self());
            struct timespec sp;
            clock_gettime(CLOCK_REALTIME, &sp);
            sp.tv_sec += 2;
            timeout = condition_timedwait(&pool->ready, &sp);
            if ( timeout == ETIMEDOUT ) {
                timeout = 1;
                break;
            }
        }
        pool->idle--;
        // 等到任务
        if ( pool->first != NULL ) {
            task_t *newtask = pool->first;            
            pool->first = newtask->next;
            //防止任务执行太长时间导致别别的任务不能操作队列
            condition_unlock(&pool->ready);
            (newtask->run)(newtask->arg);
            condition_lock(&pool->ready);
            free(newtask);
        }

        // 退出标志位1，销毁线程池
        if ( pool->quit == 1 && pool->first == NULL ) {
            pool->counter--; 
            if ( pool->counter == 0 )
                condition_signal(&pool->ready);
            condition_unlock(&pool->ready);
            break;
        }

        // 超时处理
        if ( timeout == 1 && pool->first == NULL ) {
            condition_unlock(&pool->ready);
            break;
        }

        condition_unlock(&pool->ready);
    }
    printf("%lu thread exited\n", pthread_self());
}

// 向线程池添加任务
void threadpool_add(threadpool_t *pool, void *(*run)(void*),void* arg)
{
    task_t *newtask = (task_t*)malloc(sizeof(task_t));
    newtask->run = run;
    newtask->arg = arg;
    condition_lock(&pool->ready);
    if ( pool->first == NULL ) {
        pool->first = newtask;
    } else {
        pool->last->next = newtask;
    }
    pool->last = newtask;
    if ( pool->idle > 0 ) {
        condition_signal(&pool->ready);
    } else if ( pool->counter < pool->max_threads ) {
        pthread_t tid;
        pthread_create(&tid, NULL, route, (void*)pool);
        pool->counter++;
    }
    condition_unlock(&pool->ready);
}

// 销毁线程池
void threadpool_destroy(threadpool_t *pool)
{
    condition_lock(&pool->ready);
    if ( pool->quit == 1 )  {
        condition_unlock(&pool->ready);
        return ;
    }
    pool->quit = 1;

    if ( pool->idle > 0 )
        condition_broadcast(&pool->ready);
    while ( pool->counter > 0 ) {
        condition_wait(&pool->ready); 
    }

    condition_destroy(&pool->ready);
    condition_unlock(&pool->ready);
}

