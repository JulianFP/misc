#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if(fork() == 0) printf("The PID group of the child is %d\n", getpgid(0));
    else printf("The PID group of the parent is %d\n", getpgid(0));
    return EXIT_SUCCESS;
}
