#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>

// thread linked list
typedef struct semaphoreQueue{
    pthread_t* process;
    struct semaphoreQueue* next;
}SemaphoreQueue;

// semaphore type
typedef struct{
    SemaphoreQueue* next;
    volatile atomic_int size;
    int cost;
    pthread_mutex_t mutex;
}Semaphore;

/*
 * Remove the thread from the linked list, release the semaphore (increase)
 */
void sem_remove(Semaphore* sem);
/*
 * Push the thread in the linked list
 */
void sem_push(Semaphore* sem, pthread_t *thread);
/*
 * Initialize the semaphore
 */
void sem_init(Semaphore* sem, int initSize, int initCost);
/*
 * Decrease the semaphore. Handle the thread status (pause)
 */
void sem_access(Semaphore* sem, pthread_t *thread, int* instanceId);

#endif // SEMAPHORE_H
