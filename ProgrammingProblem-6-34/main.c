/*
 ============================================================================
 Name        : ProgrammingProblem-6-33.c
 Author      : Supra
 Version     :
 Copyright   : -
 Description : Semaphore test
 ============================================================================
 */

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>
#include <unistd.h>
#include "Semaphore.h"
#include <signal.h>

#define MAX_RESOURCES 5
int available_resources = MAX_RESOURCES;

Semaphore mySemaphore;
pthread_t myInstance[10];

/*
 * This is the wake up handler
 */
void signal_handler(int sig) {
    printf("restart thread \n");
    /*
     * I wouldn't want to use a signal handler but i have no choice...
     * SIGSTOP/SIGCONT can be used only on full process, not on thread.
     * Signal handler look unavoidable.
     * I can't pass data to it... for example, instance id variable. All this is not powerfull
     */
}

/*
 * This is the thread
 */
void* lauchInstance(void* param){
    int* instanceId = (int *)param; // id of the instance

    printf("Instance %d created\n", *instanceId); // welcome msg

    sem_access(&mySemaphore, &myInstance[*instanceId], instanceId); // push the semaphore

    signal(SIGUSR1, signal_handler); // wake up point in case the semaphore pause this thread
    printf("Instance %d just ending\n", *instanceId); // bye msg

    sem_remove(&mySemaphore); // pop the semaphore
    free(instanceId); // prevent memory leaks
    pthread_exit (0); // exit thread
}

int main(void) {

    // initialize my semaphore
    sem_init(&mySemaphore, 4, 3);

    // prepare thread creation
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // initialize all thread
    for(int i = 0 ; i < 10 ; i++ ){
        int* number = malloc(sizeof(int));
        *number = i;
        pthread_create(&myInstance[i], &attr, lauchInstance, number);
    }

    // join all thread
    for(int i = 0 ; i < 10 ; i++ ){
        pthread_join(myInstance[i], NULL);
    }

    return EXIT_SUCCESS;
}
