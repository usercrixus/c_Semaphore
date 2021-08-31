#include "Semaphore.h"
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <signal.h>

void sem_remove(Semaphore* sem){
    pthread_mutex_lock(&sem->mutex); // prevent race condition
    if(sem->next != NULL){
        //prevent leak memory
        SemaphoreQueue* buff;
        buff = sem->next;
        //get next thread
        sem->next = sem->next->next;
        //wakeup process
        pthread_kill(*buff->process, SIGUSR1);
        //released the node
        free(buff);
        //increment semaphore
        atomic_fetch_add(&sem->size, sem->cost);
    }else{
        //no queue so just increment semaphore
        atomic_fetch_add(&sem->size, sem->cost);
    }
    pthread_mutex_unlock(&sem->mutex); // end of critical section
}

void sem_push(Semaphore* sem, pthread_t *thread){
    pthread_mutex_lock(&sem->mutex); // prevent race condition
    // this is classic linked list algorithm
    SemaphoreQueue* toPush = (SemaphoreQueue*)malloc(sizeof(SemaphoreQueue));
    toPush->process = thread;

    if(sem->next == NULL){
        sem->next = toPush;
    }else{
        SemaphoreQueue* buffer = sem->next;
        while(buffer->next != NULL){
            buffer = buffer->next;
        }
        buffer->next = toPush;
    }
    pthread_mutex_unlock(&sem->mutex); // end of critical section
}

void sem_access(Semaphore* sem, pthread_t *thread, int* instanceId){

    if(atomic_fetch_sub(&sem->size, sem->cost) < sem->cost ){ // semaphore is full, interupt the thread and push it to the queue
        sem_push(sem, thread);
        printf("Instance %d wait\n", *instanceId);
        pause();
    }else { // semaphore isn't full and not blocking.
        sleep(1); // just to test
    }
}

void sem_init(Semaphore* sem, int initSize, int initCost){
    // size of semaphore
    sem->size = initSize;
    // cost (decrease value) for each process
    sem->cost = initCost;
}

