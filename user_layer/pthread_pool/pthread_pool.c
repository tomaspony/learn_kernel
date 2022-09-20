#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <error.h>

struct task {
    struct task *next;
    void *(*task)(void *);
    void *arg;
};

#define MAX_ACTIVE_THREADS 10
#define MAX_WAITING_TASKS 100

struct thread_pool {
    pthread_mutex_t lock;
    pthread_cond_t cond;
    struct task *task_list;
    pthread_t *pids;
    unsigned int active_threads;
    unsigned int waitting_tasks;
    int shutdown;
};

int init_pool(struct thread_pool *pool, unsigned int threads_num)
{
    int i;
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->cond, NULL);

    pool->shutdown = 0;
    pool->task_list = malloc(sizeof(struct task));
    pool->pids = malloc(sizeof(pthread_t*) * MAX_ACTIVE_THREADS);

    if (!pool->task_list || !pool->pids) {
        printf("malloc failed\n");
        return -1;
    }

    pool->task_list->next = NULL;
    
    pool->waitting_tasks = 0;
    pool->active_threads = threads_num;

    for (i = 0; i < threads_num; i++) {
        if (pthread_create(pool->pids[i], NULL, routine, (void *)pool)) {
            perror("pthread_create failed\n");
            return -1;
        }
    }

    return 0;
}

int add_task(struct thread_pool *pool, void *(*task)(void *), void *arg)
{
    struct task *new_task = malloc(sizeof(struct task));
    struct task *tmp;
    
    if (!new_task) {
        printf("malloc failed\n");
        return -1;
    }

    new_task->arg = arg;
    new_task->task = task;

    pthread_mutex_lock(&pool->lock);

    if (pool->waitting_tasks >= MAX_WAITING_TASKS) {
        pthread_mutex_unlock(&pool->lock);
        free(new_task);
        printf("too many tasks\n");
        return 1;
    }

    tmp = pool->task_list;
    while (!tmp->next)
        tmp = tmp->next;

    tmp->next = new_task;
    pool->waitting_tasks++;

    pthread_mutex_unlock(&pool->lock);
    pthread_cond_signal(&pool->signal);

    return 0;
}

int add_thread(struct thread_pool *pool, unsigned int addition_thread_num)
{
    unsigned int actual_increment;
    unsigned int total_threads = pool->active_threads + addition_thread_num;
    int i, ret = 0;

    for (i = 0; i < total_threads && i < MAX_ACTIVE_THREADS; i++) {
        if (pthread_create(pool->pids[i], NULL, routine, (void *)pool)) {
            perror("pthread_create failed\n");
            ret = -1;
            break;
        }
        actual_increment++;
    }

    pool->active_threads += actual_increment;
    return ret;
}


int remove_thread(struct thread_pool *pool, unsigned int remove_threads_num)
{
    int remain_threads = pool->active_threads - remove_threads_num;
    int i;
    remain_threads > 0 : remain_threads : 1;
    int errno;
    int actual_cancel = 0;
    
    for (i = pool->active_threads - 1; i > remain_threads - 1; i--) {
        if (pthread_cancel(pool->pids[i])) {
            perror("pthread_cancel failed\n");
            break;
        }
        actual_cancel++;
    }

    if (i > remain_threads - 1) {
        return  -1;
    }

    pool->active_threads -= actual_cancel;

    return 0;
}

int destory_pool(struct thread_pool *pool)
{
    
}

int main(int argc, char *argv[])
{
    
}












