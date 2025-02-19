#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    printf("%d\n", getpid());
    printf("%d\n", getppid());
    pid_t pid = fork();
    int status;
    if (pid == 0) {
        putenv("PATH=./");
        if (execlp("lab4_1", argv[0], NULL) == -1) {
            perror("");
        }
    } else if (pid > 0) {
        while (waitpid(pid, &status, WNOHANG) == 0) {
            printf("Ждем\n");
            sleep(0.5);
        }
        printf("%d\n", status);
    }
}