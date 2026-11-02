
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define THREAD_COUNT 5
#define RESOURCE_LIMIT 2

static sem_t resource_sem;

void *thread_function(void *arg)
{
    int thread_id = *(int *)arg;

    printf("Thread %d waiting for resource...\n", thread_id);

    sem_wait(&resource_sem);

    // Critical Section
    printf("Thread %d acquired resource\n", thread_id);

    sleep(2);

    printf("Thread %d releasing resource\n", thread_id);

    sem_post(&resource_sem);

    return NULL;
}

int main(void)
{
    pthread_t threads[THREAD_COUNT];
    int ids[THREAD_COUNT];

    sem_init(&resource_sem, 0, RESOURCE_LIMIT);

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        ids[i] = i + 1;

        pthread_create(
            &threads[i],
            NULL,
            thread_function,
            &ids[i]
        );
    }

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&resource_sem);

    return 0;
}



