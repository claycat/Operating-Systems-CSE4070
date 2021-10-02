/* additional system calls which uses */
#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

int
main (int argc, char **argv) 
{
    if(argc!=5)
    {
        printf("Invalid usage : argument number should be 4\n");
        return EXIT_FAILURE;
    }
    int a, b, c, d;
    a = atoi(argv[1]);
    b = atoi(argv[2]);
    c = atoi(argv[3]);
    d = atoi(argv[4]);

    if(a <= 0)
    {
        printf("Value of first argument should be natural number\n");
        return EXIT_FAILURE;
    }

    printf("%d %d\n", fibonacci(a), max_of_four_int(a,b,c,d));
  


    return EXIT_SUCCESS;
}