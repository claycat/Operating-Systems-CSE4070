#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/process.h"
#include <devices/shutdown.h>


static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  lock_init(&filesys_lock);
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  check_valid((void*)f->esp);
  int sys_code = *(int*)f->esp;

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
      /* case like 0x20101234 */
      check_valid((int*)f->esp + 1);
      char *cmd_line = (char*)(*((int*)f->esp + 1));
      if(cmd_line == NULL 
        || !is_user_vaddr(cmd_line)
        || pagedir_get_page(thread_current()->pagedir, cmd_line) == NULL
        || !is_user_vaddr(f->esp + 1)
      )
      {
        f->eax = -1;
        exit(-1);
        break;
      }

      f->eax = exec(cmd_line);
      break;
    }

    case SYS_WAIT:
    {
      check_valid((int*)f->esp + 1);
      pid_t child_tid = *((int*)f->esp+1);
      f->eax = wait(child_tid);
      break;
    }

    case SYS_CREATE:
    {
      check_valid((int*)f->esp + 1);
      check_valid((int*)f->esp + 2);
      char *file_name = (char*)(*((int*)f->esp + 1));
      unsigned initial_size = (unsigned)(*((int*)f->esp + 2));

      if(file_name == NULL
        || !is_user_vaddr(file_name)
        || pagedir_get_page(thread_current()->pagedir, file_name) == NULL
        || !is_user_vaddr(f->esp + 1)
      )
      {
        f->eax = -1; 
        exit(-1);
        break;
      }

      f->eax = create(file_name, initial_size);
      break;
    }

    case SYS_REMOVE:
    {
      check_valid((int*)f->esp + 1);
      char *file_name = (char*)(*((int*)f->esp + 1));

      if(file_name == NULL
        || !is_user_vaddr(file_name)
        || pagedir_get_page(thread_current()->pagedir, file_name) == NULL
        || !is_user_vaddr(f->esp + 1)
      )
      {
        f->eax = -1; 
        exit(-1);
        break;
      }

      f->eax = remove(file_name);
      break;
    }

    case SYS_OPEN:
    {
      check_valid((int*)f->esp + 1);
      char *file_name = (char*)(*((int*)f->esp + 1));

      if(file_name == NULL
        || !is_user_vaddr(file_name)
        || pagedir_get_page(thread_current()->pagedir, file_name) == NULL
        || !is_user_vaddr(f->esp + 1)
      )
      {
        f->eax = -1; 
        exit(-1);
        break;
      }

      f->eax = open(file_name);
      break;
    }

    case SYS_FILESIZE: 
    {
      check_valid((int*)f->esp + 1);
      int fd = (int)(*((int*)f->esp + 1));
      f->eax = filesize(fd);
      break;
    }

    case SYS_READ:
    {
      check_valid((int*)f->esp + 1);
      check_valid((int*)f->esp + 2);
      check_valid((int*)f->esp + 3);

      int fd = *((int*)f->esp + 1);
      void* buffer = (void*)(*((int*)f->esp + 2));
      unsigned size = *((unsigned*)f->esp + 3);
      f->eax = read(fd, buffer, size);
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
      f->eax = write(fd, buffer, size);
      break;
    }

    case SYS_SEEK:
    {
      check_valid((int*)f->esp + 1);
      check_valid((int*)f->esp + 2);
      int fd = (int)(*((int*)f->esp + 1));
      unsigned position = (unsigned)(*((int*)f->esp + 2));
      seek(fd, position);
      break;
    }
    case SYS_TELL:
    {
      check_valid((int*)f->esp + 1);
      int fd = (int)(*((int*)f->esp + 1));
      f->eax = tell(fd);
      break;
    }
    case SYS_CLOSE:
    {
      check_valid((int*)f->esp + 1);
      int fd = (int)(*((int*)f->esp + 1));
      close(fd);
      break;
    }


    case SYS_FIBO:
    {
      check_valid((int*)f->esp + 1);
      int a = *((int*)f->esp + 1);
      f->eax = fibonacci(a);
      break;
    }

    case SYS_MAX:
    {
      
      check_valid((int*)f->esp + 1);
      check_valid((int*)f->esp + 2);
      check_valid((int*)f->esp + 3);
      check_valid((int*)f->esp + 4);

      int a = *((int*)f->esp + 1);
      int b = *((int*)f->esp + 2);
      int c = *((int*)f->esp + 3);
      int d = *((int*)f->esp + 4);
      f->eax = max_of_four_int(a, b, c, d);
      break;
    }

    default:
      break;
  }
}

void 
check_valid (void* addr) 
{

  if(addr == NULL)
  {
    exit(-1);
  }
  if(!(0x08048000 < addr && addr < PHYS_BASE))
  {
    exit(-1);
  }
  if(pagedir_get_page(thread_current()->pagedir, addr) == NULL)
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
  cur->exit_code = status;
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

int
read(int fd, void* buffer, unsigned size)
{
  int i = -1;
  if(fd==0){
    for(i = 0; i < (int)size; i++)
    {
      *(char*)(buffer+i)=input_getc();
    }
  }
  return i;
}

pid_t 
exec(const char* cmd_line)
{
  /* create child process */
  tid_t id = process_execute(cmd_line);
  
  /* find child process */
  struct thread *child_process = get_child_pid(id);
  /* sema_down - if value is negative wait */
  sema_down(&(child_process->sema_load));
  
  /* waits until start_process finishes loading */
  if(child_process->load_status)
  {
    return id;
  }
  else
  {
    return -1;
  } 
 
}

int 
wait (pid_t tid)
{
  return process_wait(tid);
}

int 
fibonacci(int a)
{
  if(a == 1 || a == 2) return 1;

  int prev = 1, mid= 1, next = 0;

  for(int i = 3; i <= a; i++)
  {
    next = prev + mid;
    prev = mid;
    mid = next;
  }
  return next;
}

int 
max_of_four_int(int a, int b, int c, int d)
{
  int ret = a;
  if(ret < b) ret = b;
  if(ret < c) ret = c;
  if(ret < d) ret = d;

  return ret;
}

bool
create (const char *file, unsigned initial_size)
{
  
  return true;
}

bool
remove (const char *file)
{
  return true;
}

int
open (const char *file)
{
  /* open file */
  struct file *open_file = filesys_open(file);

  /* if file does not exist return -1 */
  if(open_file == NULL) return -1;

  /* add file to file descriptors */
  int file_fd = add_file_to_fd(open_file);
  
  return file_fd;
}

int
filesize(int fd)
{
  return 0;
}

void
seek (int fd, unsigned position)
{

}

unsigned 
tell (int fd)
{
  return 0;
}

void
close (int fd)
{

}