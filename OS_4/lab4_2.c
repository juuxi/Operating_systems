#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    printf("родительский процесс начал работу\n");
    printf("Индентифекатор текущего процесса - %d\n", getpid());
    printf("Индентифекатор родительского процесса - %d\n", getppid());
    pid_t pid = fork();
    int status;
    if (pid == 0) {
        putenv("PATH=./");
        if (execlp("lab4_1", argv[0], argv[1], argv[2], argv[3], NULL) == -1) {
            perror("");
        }
    } else if (pid > 0) {
        while (waitpid(pid, &status, WNOHANG) == 0) {
            printf("Ждем\n");
            usleep(500000);
        }
        printf("Код завершения дочернего процесса - %d\n", WEXITSTATUS(status));
    }
    printf("родительский процесс закончил работу\n");
}