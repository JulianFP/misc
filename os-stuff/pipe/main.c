#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int fd[2];
    pipe(fd);
    if(fork() == 0){
        close(fd[0]);
        char string1[] = "hello";
        char string2[] = "world!";
        write(fd[1], string1, (sizeof(string1)+1));
        sleep(10);
        write(fd[1], string2, (sizeof(string2)+1));
        close(fd[1]);
        exit(0);
    }
    else{
        close(fd[1]);
        char readbuffer[100];
        read(fd[0], readbuffer, sizeof(readbuffer));
        printf("%s\n", readbuffer);
        close(fd[0]);
        exit(0);
    }
    return 0;
}
