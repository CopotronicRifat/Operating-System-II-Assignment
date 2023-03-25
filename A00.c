// Assignment 00
// Submitted by: S M Rafiuddin
// CWID: A20387294


#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


void  Child1(int x);
void  Child2(int x);
void  Collatz(int x, int y);

int  main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("You must provide the argument in the command line.\n");
        exit(1);
    }

    int n = atoi(argv[1]);


    if(n <= 0 || n >= 40)
    {
        printf("Please enter a number greater than 0 and less than 40 to run the PROGRAM.\n");
        exit(1);
    }

    pid_t  pid;
    pid = fork();

    if (pid == 0)
    {
        Child1(n);
    }
    else
    {
        int returnStatus;
        waitpid(pid, &returnStatus, 0);

        if (returnStatus == 0)
            printf("Children complete\n\n");
    }
    return 0;
}

// Child 01

void Child1(int x)
{
    int num = x;

    printf("\nFrom Child 1 init() n=%d\n",num);
    Collatz(num, 1);

    pid_t  pid1;
    pid1 = fork();

    if (pid1 == 0)
    {
        Child2(x);
    }
    else
    {
        int returnStatus;
        waitpid(pid1, &returnStatus, 0);
    }
}

// Child 02

void Child2(int y)
{
    int num = y + 3;
    printf("\nFrom Child 2 init() n=%d\n",num);
    Collatz(num, 2);
}


// Collatz Conjecture

void Collatz(int n, int child)
{
    while(n != 1)
    {
        if(n%2 == 0)
        {
            n = n / 2;
            printf("From Child %d n=%d\n", child, n);
        }
        else
        {
            n = 3 * n + 1;
            printf("From Child %d n=%d\n", child, n);
        }
    }
}




