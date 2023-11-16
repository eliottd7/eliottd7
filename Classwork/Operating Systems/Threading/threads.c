#include "threads.h"
#include "ec440threads.h"

#define FAIL_AND_EXIT(PARAM_msg) do { \
            perror(PARAM_msg); \
            exit(EXIT_FAILURE); \
        } while(0)

#define ASSERT(PARAM_val, PARAM_msg) do { \
            if(PARAM_val != 0) \
                FAIL_AND_EXIT(PARAM_msg); \
        } while(0)

// Global metadata pointer is on stack, but actual data is put on heap
static global_data* metadata;

void pthread_exit_wrapper()
{
    unsigned long int res;
    asm("movq %%rax, %0\n":"=r"(res));
    pthread_exit((void *) res);
}

// Create threads
int pthread_create( pthread_t *thread,
                    const pthread_attr_t *attr,
                    void *(*start_routine)(void*),
                    void *arg)
{

    // Convert main to thread on first call
    static bool is_first_call = true;
    if(is_first_call){
        is_first_call = false;
        setup_metadata();
        ASSERT(setjmp(metadata->TCB[MAIN_THREAD].jump_buffer), "main setjmp");
        ualarm((useconds_t)SCHED_QUANTUM_US, 0);
    }

    // Find next available thread
    if((metadata->thread_count + 1) > MAX_THREAD_COUNT){
        FAIL_AND_EXIT("Too many threads");
    }
    int new_thread_index = metadata->current_thread;
    while(metadata->TCB[++new_thread_index].status != TS_UNUSED);
    metadata->TCB[new_thread_index].status = TS_BLOCKED;
    ++metadata->thread_count;

    // Set up new thread
    thread_ControlBlock* new_thread = &(metadata->TCB[new_thread_index]);
    if((new_thread->base_pointer = malloc(STACK_SIZE_BYTES)) == NULL){
        FAIL_AND_EXIT("malloc");
    }
    new_thread->stack_pointer = new_thread->base_pointer + STACK_SIZE_BYTES; //move stack pointer to top
    void (*exit_pointer)(void*) = &pthread_exit_wrapper; //pthread_exit on ret'n addr...
    new_thread->stack_pointer -= sizeof(exit_pointer); //... of new stack
    memcpy(new_thread->stack_pointer, &exit_pointer, sizeof(exit_pointer));

    // New thread jump buffer setup
    new_thread->jump_buffer->__jmpbuf[JB_RSP] = ptr_mangle(
        (unsigned long int)new_thread->stack_pointer);
    new_thread->jump_buffer->__jmpbuf[JB_R12] = (unsigned long int)start_routine;
    new_thread->jump_buffer->__jmpbuf[JB_R13] = (unsigned long int)arg;
    new_thread->jump_buffer->__jmpbuf[JB_PC] = ptr_mangle((unsigned long int)start_thunk);

    // Place thread ID into argument
    *thread = (pthread_t)new_thread_index;

    // Update status
    new_thread->status = TS_READY;

    return 0;
}

// Terminate calling thread
void pthread_exit(void *value_ptr){

    //end quantum early, prevents SIGALRM going off mid-exit
    ualarm(0,0);
    lock();

    // Update thread status
    pthread_t self = pthread_self();

    if(metadata->TCB[self].base_pointer){
        free(metadata->TCB[self].base_pointer);
        metadata->TCB[self].base_pointer = NULL;
        metadata->TCB[self].stack_pointer = NULL;
    }
    metadata->TCB[self].status = TS_EXITED;
    metadata->TCB[self].retval = value_ptr;
    metadata->TCB[self].jump_buffer->__jmpbuf[JB_RSP] = 0;
    metadata->TCB[self].jump_buffer->__jmpbuf[JB_R12] = 0;
    metadata->TCB[self].jump_buffer->__jmpbuf[JB_R13] = 0;
    metadata->TCB[self].jump_buffer->__jmpbuf[JB_PC] = 0;

    if(metadata->TCB[self].joined_head){
        join_list* iter = metadata->TCB[self].joined_head;
        join_list* temp = iter;
        while(iter != NULL){
            iter = iter->next;
            metadata->TCB[temp->thread].status = TS_READY;
            free(temp);
            temp = iter;
        }
    }

    // Update metadata struct
    --metadata->thread_count;

    unlock();

    // Make main thread "spin" until all child threads are done
    if(pthread_self() == MAIN_THREAD){
        while(metadata->thread_count > 0){
            schedule(SIGALRM); //reschedule so it's not just busy waiting
        }
        printf("EXITING.");
        free(metadata);
        exit(0);
    }

    // All other threads won't re-schedule due to TS_EXITED status
    schedule(SIGUSR2);
    __builtin_unreachable();
}

// Return thread ID of calling thread

inline pthread_t pthread_self(void){
    return metadata->current_thread;
}


// Schedule and manage threads
void schedule(int signal){

    //Ensure alarm is not the reason schedule() returns early
    ualarm(0,0);

    // Fetch thread data
    pthread_t this = metadata->current_thread;

    //Update status if thread is running
    if(metadata->TCB[this].status == TS_RUNNING){
        metadata->TCB[this].status = TS_READY;
    }

    // setjmp returns 1 when longjmp() is invoked- see longjmp() below vv
    // and 0 otherwise
    if(setjmp(metadata->TCB[this].jump_buffer) != 0){

        // Return sends thread back to execution at signal raise point
        return;
    }

    // Check thread count
    if(metadata->thread_count <= 0){
        FAIL_AND_EXIT("Too few threads");
    }

    // Round Robin search for next thread
    int search = this+1;
_search_again:
    while(metadata->TCB[search].status != TS_READY){
        search = (search == MAX_THREAD_COUNT)? 0 : search + 1;
    }

    if(metadata->TCB[search].status == TS_EXITED){
        goto _search_again;
    }

    metadata->current_thread = search;
    if(metadata->TCB[search].status == TS_READY){
        metadata->TCB[search].status = TS_RUNNING;
    }

    // Reset timer
    ualarm((useconds_t)SCHED_QUANTUM_US, 0);

    // 1 will be returned from setjmp() after longjmp() is done, see above ^^
    longjmp(metadata->TCB[search].jump_buffer, 1);
    __builtin_unreachable();
}

// (Set up and) return global metadata using static variables
global_data* setup_metadata(void){

    // Put global data in memory, not local stack
    metadata = (global_data*)malloc(sizeof(global_data));

    // Thread management data
    metadata->thread_count = 1; //starts at 1 because of main thread
    metadata->current_thread = MAIN_THREAD; //dedicate TCB[0] to main thread
    metadata->TCB[MAIN_THREAD].status = TS_RUNNING;
    metadata->TCB[MAIN_THREAD].stack_pointer = NULL;

    // Populate all threads with IDs
    for(int i = 1; i < MAX_THREAD_COUNT; i++){
        metadata->TCB[i].status = TS_UNUSED;
        metadata->TCB[i].stack_pointer = NULL;
        metadata->TCB[i].thread_id = (pthread_t)i; //pthread_t arithmetic
    }

    // Set up signal handlers
    metadata->sa.sa_handler = &schedule; //make schedule() signal handler
    metadata->sa.sa_flags = SA_NODEFER; //don't block signals from delivery
    sigprocmask(SIG_UNBLOCK, &(metadata->sa.sa_mask), NULL); //unblock all sig's
    ASSERT(sigaction(SIGALRM, &(metadata->sa), NULL), "sigaction SIGALRM");
    ASSERT(sigaction(SIGUSR1, &(metadata->sa), NULL), "sigaction SIGUSR1");

    return metadata;
}

sigset_t s;

// Lock thread using signal masking
void lock(void){
    sigfillset(&s);
    sigprocmask(SIG_BLOCK, &s, NULL);
}

// Unlock thread by unblocking signal mask
void unlock(void){
    sigprocmask(SIG_UNBLOCK, &s, NULL);
}

// "Wait" for another thread to finish
int pthread_join(pthread_t thread, void **value_ptr){

    if(thread >= MAX_THREAD_COUNT) return -1;
    if(thread < 0) return -1;
    if(metadata->TCB[thread].status == TS_UNUSED) return -1;

    // Must see if thread already exited
    if(metadata->TCB[thread].status == TS_EXITED){
        goto end;
    }

    lock();

    // Add to join list
    pthread_t self = pthread_self();
    join_list* end = metadata->TCB[thread].joined_tail;
    if(end == NULL){
        end = (join_list*)malloc(sizeof(join_list));
        end->thread = self;
        end->next = NULL;
        metadata->TCB[thread].joined_head = end;
        metadata->TCB[thread].joined_tail = end;
    }
    else {
        end->next = (join_list*)malloc(sizeof(join_list));
        end = end->next;
        end->thread = self;
        end->next = NULL;
        metadata->TCB[thread].joined_tail = end;
    }

    // Block state until ready
    metadata->TCB[self].status = TS_BLOCKED;

    unlock();
    schedule(SIGUSR1);

end:
    *(unsigned long int**)value_ptr = (unsigned long*)metadata->TCB[thread].retval;
    return 0;
}


int sem_init(sem_t *sem, int pshared, unsigned value){
    lock();

    if(sem == NULL){
        sem = (sem_t*)malloc(sizeof(sem_t));
        sem->value = value;
        sem->head = NULL;
        sem->last = NULL;
        sem->init = 1;
        unlock();
        return 0;
    }
    else if (sem->init){
        unlock();
        return -1;
    }
    else {
        sem->value = value;
        sem->init = 1;

        unlock();
        return 0;
    }
}

int sem_wait(sem_t *sem){
    lock();

    if(sem->value > 0){
        sem->value -= 1;
        return 0;
    }

    if(sem->head == NULL){
        sem->head = (sem_queue*)malloc(sizeof(sem_queue));
        sem->head->next = NULL;
        sem->last = sem->head;
        sem->head->thread = pthread_self();
    }
    else {
        sem->last->next = (sem_queue*)malloc(sizeof(sem_queue));
        sem->last = sem->last->next;
        sem->last->thread = pthread_self();
        sem->last->next = NULL;
    }

    metadata->TCB[pthread_self()].status = TS_BLOCKED;
    unlock();

    schedule(SIGUSR1);

    return 0;
}

int sem_post(sem_t *sem){
    lock();

    if(sem->head != NULL){
        metadata->TCB[sem->head->thread].status = TS_READY;
        sem_queue* temp = sem->head;
        sem->head = sem->head->next;
        free(temp);
    }
    else {
        sem->value += 1;
    }

    unlock();
    return 0;
}

int sem_destroy(sem_t *sem){
    lock();

    sem->init = 0;

    if(sem->head){
        sem_queue* iter = sem->head;
        sem_queue* temp = sem->head;
        while(iter){
            iter = sem->head->next;
            metadata->TCB[temp->thread].status = TS_READY;
            free(temp);
            temp = iter;
        }
    }
    free(sem);

    unlock();
    return 0;
}
