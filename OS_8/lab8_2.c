#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/queue.h>
#include <sys/stat.h>

int flag1 = 0;
int client_sock;

void * func1() {
    printf("поток начал работу\n");
    int rcv_msg = -15;
    while(flag1 == 0) {
        struct sockaddr_un addr;
        addr.sun_family = AF_UNIX;
        socklen_t slen = sizeof(addr);
        strncpy(addr.sun_path,"socket.soc",sizeof(addr.sun_path) - 1);
        int rv = recvfrom(client_sock, &rcv_msg, sizeof(&rcv_msg), 0, &addr, &slen);
        if (rv == -1)
            perror("rcv");
        printf("%d\n", rcv_msg);
        sleep(1);
    }
    printf("поток закончил работу\n");
}

int main() {
    printf("программа сервера работу\n");
    pthread_t id1;

    client_sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    fcntl(client_sock, F_SETFL, O_NONBLOCK);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path,"socket.soc",sizeof(addr.sun_path) - 1);
    bind(client_sock, &addr, sizeof(addr));
    int optval = 1;
    setsockopt(client_sock, SOL_SOCKET, SO_REUSEADDR,
        &optval, sizeof(optval));

    pthread_create(&id1, NULL, func1, NULL);
    printf("программа ждет нажатия клавиши\n");
    getchar();
    printf("клавиша нажата\n");
    flag1 = 1;
    pthread_join(id1, NULL);

    printf("программа сервера работу\n");
}