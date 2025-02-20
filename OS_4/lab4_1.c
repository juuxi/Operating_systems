#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


void main(int argc, char* argv[])  {
    printf("дочерний процесс начал работу\n");
    for (int i = 0; i < 10; i ++) {
        printf("Индентифекатор текущего процесса - %d\n", getpid());
        printf("Индентифекатор родительского процесса - %d\n", getppid());
        printf("%s \n", argv[0]);
        printf("%s \n", argv[1]);
        printf("%s \n", argv[2]);
        printf("%s \n", argv[3]);
        sleep(1);
    }
    printf("дочерний процесс закончил работу\n");
    exit(25);
}