#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void main(int argc, char* argv[])  {
    printf("дочерний процесс начал работу\n");
    for (int i = 0; i < 10; i ++) {
        printf("%d\n", getpid());
        printf("%d\n", getppid());
        sleep(1);
    }
    exit(25);
}