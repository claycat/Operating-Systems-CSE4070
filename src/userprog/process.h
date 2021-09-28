#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/synch.h"
#include "threads/thread.h"


tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);
void parse_arguments(const char* filename, void **esp);
struct thread *get_child_pid (int pid);
void remove_child_process (struct thread *child_proc);

#endif /* userprog/process.h */
