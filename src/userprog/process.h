#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/synch.h"
#include "threads/thread.h"

struct thread_data
{
    char *fn_copy;
    struct semaphore thread_sema;
};

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);
void parse_arguments(const char* filename, void **esp);


#endif /* userprog/process.h */
