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
#include <ctype.h>

int flag_rcv = 0;
int client_sock;
struct sockaddr_un addr;

void * func1() {
    printf("поток приема начал работу\n");
    char rcv_msg[50];
    while(flag_rcv == 0) {
        socklen_t slen = sizeof(addr);
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path,"socket.soc",sizeof(addr.sun_path) - 1);
        int rv = recvfrom(client_sock, &rcv_msg, sizeof(rcv_msg), 0, (struct sockaddr*)&addr, &slen);
        char* iter = rcv_msg;
        long val = 0;
        int i = 0;
        while (!isdigit(*iter)) {
            iter++;
        }
        while (isdigit(*iter)) {
            i = i*10+*iter-'0';
            iter++;
        }
        while (!isdigit(*iter)) {
            iter++;
        }
        while (isdigit(*iter)) {
            val = val*10+*iter-'0';
            iter++;
        }
        if (rv == -1)
            perror("rcv");
        else {
            printf("%s\n", rcv_msg);
            printf("%d\n", i);
            printf("%ld\n", val);
        }
        sleep(1);
    }
    printf("поток приема закончил работу\n");
}

int main() {
    printf("программа сервера работу\n");
    pthread_t id1;

    client_sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    fcntl(client_sock, F_SETFL, O_NONBLOCK);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path,"socket.soc",sizeof(addr.sun_path) - 1);
    int rv = bind(client_sock, (struct sockaddr*)&addr, sizeof(addr));
    if (rv == -1) {
        perror("bind");
    }
    int optval = 1;
    setsockopt(client_sock, SOL_SOCKET, SO_REUSEADDR,
        &optval, sizeof(optval));

    pthread_create(&id1, NULL, func1, NULL);
    printf("программа ждет нажатия клавиши\n");
    getchar();
    printf("клавиша нажата\n");
    flag_rcv = 1;
    pthread_join(id1, NULL);

    close(client_sock);
    unlink("socket.soc");

    printf("программа сервера работу\n");
}