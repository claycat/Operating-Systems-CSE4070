#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include <stdbool.h>
#include "lib/user/syscall.h"

void syscall_init (void);
void check_valid (void* addr);
void halt (void);
void exit (int status);
int write (int fd, const void *buffer, unsigned size);
int read(int fd, void* buffer, unsigned size);
pid_t exec (const char *cmd_line);
pid_t wait (pid_t tid);
int fibonacci(int a);
int max_of_four_int(int a, int b, int c, int d);
#endif /* userprog/syscall.h */
