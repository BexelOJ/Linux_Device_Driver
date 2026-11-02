
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ITERATIONS 1000000

static int shared_counter = 0;

static pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;

void *thread_function(void *arg)
{
    int thread_id = *(int *)arg;

    for (int i = 0; i < ITERATIONS; i++)
    {
        pthread_mutex_lock(&mutex_lock);

        // Critical Section
        shared_counter++;

        pthread_mutex_unlock(&mutex_lock);
    }

    printf("Thread %d completed\n", thread_id);

    return NULL;
}

int main(void)
{
    pthread_t thread1, thread2;

    int id1 = 1;
    int id2 = 2;

    pthread_create(&thread1, NULL, thread_function, &id1);
    pthread_create(&thread2, NULL, thread_function, &id2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("\nFinal Counter = %d\n", shared_counter);
    printf("Expected      = %d\n", 2 * ITERATIONS);

    pthread_mutex_destroy(&mutex_lock);

    return 0;
}



