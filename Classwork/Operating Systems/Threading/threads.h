#ifndef THREADS_H
#define THREADS_H

/************************/
/*  Includes and macros */
/************************/

#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

// Registers from setjmp.h
#define JB_RBX  0
#define JB_RBP  1
#define JB_R12  2
#define JB_R13  3
#define JB_R14  4
#define JB_R15  5
#define JB_RSP  6
#define JB_PC   7

// Constants
#define MAX_THREAD_COUNT    128
#define STACK_SIZE_BYTES    32767   // 15 bits of all 1's = 0x7FFF
#define SCHED_QUANTUM_US    50000   // microseconds between schedule() = 50ms
#define MAIN_THREAD         0       // dedicate TCB[0] to main thread

// Structs
typedef enum thread_status {
    TS_UNUSED = 0,  // 0; default upon thread creation before init'n
    TS_EXITED,      // 1
    TS_RUNNING,     // 2
    TS_READY,       // 3
    TS_BLOCKED      // 4; use while thread is not ready during init'n
} thread_status;

typedef struct join_list {
    struct join_list* next;
    pthread_t thread;
} join_list;

typedef struct thread_ControlBlock {
    pthread_t thread_id;
    void* stack_pointer;
    void* base_pointer;
    thread_status status;
    jmp_buf jump_buffer;
    join_list* joined_head;
    join_list* joined_tail;
    void* retval;
} thread_ControlBlock;

typedef struct global_data { //struct to manage "global" variables
    struct sigaction sa;
    thread_ControlBlock TCB[MAX_THREAD_COUNT];
    unsigned int thread_count; //count of all threads
    pthread_t current_thread; //actively running thread (arithmetic type, int)
    useconds_t time;
} global_data;

typedef struct sem_queue {
    pthread_t thread;
    struct sem_queue* next;
} sem_queue;

typedef struct sem_t {
    char init;
    unsigned value;
    unsigned max;
    sem_queue* head;
    sem_queue* last;
} sem_t;


/************************/
/*  Helper Functions    */
/************************/

global_data* setup_metadata(void);


/*****************************/
/*  Primary utility Functions*/
/*****************************/

// Create threads
int pthread_create(
    pthread_t* thread, //pointer to thread struct to populate
    const pthread_attr_t *attr, //attributes, will be NULL for this class
    void* (*start_routine)(void*), //Pointer to function of thread
    void* arg //Arguments to the start function
    );

// Terminate calling thread
void pthread_exit(void *value_ptr);

// Return thread ID of calling thread
pthread_t pthread_self(void);

// Schedule and manage threads
void schedule(int signal);

// Lock thread using mutex
void lock(void);

// Unlock thread mutex
void unlock(void);

// "Wait" for another thread to finish
int pthread_join(pthread_t thread, void **value_ptr);

// Semaphore functions
int sem_init(sem_t *sem, int pshared, unsigned value);
int sem_wait(sem_t *sem);
int sem_post(sem_t *sem);
int sem_destroy(sem_t *sem);

#endif
