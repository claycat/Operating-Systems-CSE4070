#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include <stdbool.h>

void syscall_init (void);
void check_valid (void* addr);
void halt (void);
void exit (int status);
#endif /* userprog/syscall.h */
