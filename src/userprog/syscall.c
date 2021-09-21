#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include <devices/shutdown.h>

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  int sys_code = *(int*)f->esp;
  //first check if it is valid pointer
  //check sys_code -> get number of parameters

  /*
  void halt(void)
  void exit(int status)
  pid_t exec(const char *cmd_line)
  int wait(pid_t pid)
  int read(int fd, void*buffer, unsigned size)
  int write(int fd, const void*buffer, unsigned size)
  */
  switch(sys_code){
    case SYS_HALT:
    {
      halt();
      break;
    }
    case SYS_EXIT:
    {
      check_valid((int*)f->esp + 1);
      int status = *((int*)f->esp +1);
      exit(status);
      break;
    }
    case SYS_EXEC:
    {
      check_valid((int*)f->esp + 1);
      break;
    }
    case SYS_WAIT:
    {
      check_valid((int*)f->esp + 1);
      break;
    }
    case SYS_READ:
    {
      check_valid((int*)f->esp + 1);
      check_valid((int*)f->esp + 2);
      check_valid((int*)f->esp + 3);
      break;
    }
    case SYS_WRITE:
    {
      check_valid((int*)f->esp + 1);
      check_valid((int*)f->esp + 2);
      check_valid((int*)f->esp + 3);
      
      int fd = *((int*)f->esp + 1);
      void* buffer = (void*)(*((int*)f->esp + 2));
      
      unsigned size = *((unsigned*)f->esp + 3);
      //printf("fd:%d buffer:%s bufptr:%p size:%d\n", fd, (char*)buffer, buffer, size);
      f->eax = write(fd, buffer, size);
      break;
    }
    default:
      break;

  }
}

void 
check_valid (void* addr) 
{
  if(!is_user_vaddr(addr))
  {
    exit(-1);
  }
}

void 
halt(void)
{
  shutdown_power_off();
}

void 
exit (int status)
{
  struct thread *cur = thread_current();
  cur->status = status;
  printf("%s: exit(%d)\n", thread_name() ,status);
  thread_exit();
}

int
write (int fd, const void *buffer, unsigned size)
{
  if(fd==1)
  {
    putbuf(buffer, size);
    return size;
  }
  return -1;
}