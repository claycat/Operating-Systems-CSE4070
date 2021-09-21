#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include <stdbool.h>

void syscall_init (void);
void check_valid (void* addr);
void halt (void);
void exit (int status);
int write (int fd, const void *buffer, unsigned size);
#endif /* userprog/syscall.h */
