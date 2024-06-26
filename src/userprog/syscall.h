#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include <stdbool.h>
#include "lib/user/syscall.h"

struct lock filesys_lock;

void halt (void) NO_RETURN;
void exit (int status) NO_RETURN;
pid_t exec (const char *file);
int wait (pid_t);
bool create (const char *file, unsigned initial_size);
bool remove (const char *file);
int open (const char *file);
int filesize (int fd);
int read (int fd, void *buffer, unsigned length);
int write (int fd, const void *buffer, unsigned length);
void seek (int fd, unsigned position);
int fibonacci(int a);
int max_of_four_int(int a, int b, int c, int d);
unsigned tell (int fd);
void close (int fd);
#endif /* userprog/syscall.h */
