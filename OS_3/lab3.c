#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int flag1 = 0, flag2 = 0;
int fd[2];

void * func1() {
    printf("поток 1 начал работу\n");
    long buf;
    while(flag1 == 0) {
        buf = pathconf("./", 3);
        int rv = write(fd[1], &buf, sizeof(long));
        if (rv < 0) {
            perror("Ошибка при записи\n");
        }
        sleep(1);
    }
    printf("поток 1 закончил работу\n");
}

void * func2() {
    printf("поток 2 начал работу\n");
    long buf;
    while(flag2 == 0) {
        int rv = read(fd[0], &buf, sizeof(long));
        if (rv == -1) {
            perror("Ошибка при чтении\n");
            sleep(1);
        }
        if (rv == 0) {
            printf("EOF\n");
        }
        if (rv > 0) {
            printf("%ld", buf);
            sleep(1);
        }
    }
    printf("поток 2 закончил работу\n");
}

void sig_handler(int signo) {
    close(fd[0]);
    close(fd[1]);
    exit(0);
}

void main(int argc, char* argv[]) {
    signal(SIGINT, sig_handler);
    printf("программа начала работу\n");
    pthread_t id1, id2;
    if (argv[1]) {
        if (!strcmp(argv[1], "pipe")) {
            pipe(fd);
        }
        else if (!strcmp(argv[1], "pipe2")) {
            pipe2(fd, O_NONBLOCK);
        }
        else if (!strcmp(argv[1], "fcntl")) {
            pipe(fd);
            fcntl(fd[0], F_SETFL, O_NONBLOCK);
            fcntl(fd[1], F_SETFL, O_NONBLOCK);
        }
    }
    else {
        pipe(fd);
    }
    
    pthread_create(&id1, NULL, func1, NULL);
    pthread_create(&id2, NULL, func2, NULL);
    printf("программа ждет нажатия клавиши\n");
    getchar();
    printf("клавиша нажата\n");
    flag1 = 1;
    flag2 = 1;
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
    close(fd[0]);
    close(fd[1]);
    printf("программа завершила работу\n");
}