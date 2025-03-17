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

int flag_send = 0;
int flag_receive = 0;
int server_sock;
int server_sock_2;
struct sockaddr_un addr;
struct sockaddr_un addr2;

void * func1() {
    printf("поток отправки запросов начал работу\n");
    char send_msg[50];
    int i = 1;
    while(flag_send == 0) {
        sprintf(send_msg, "Сообщение %d отправлено", i);
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path,"socket.soc",sizeof(addr.sun_path) - 1);
        int rv = sendto(server_sock, &send_msg, sizeof(send_msg), 0, (struct sockaddr*)&addr, sizeof(addr));
        if (rv == -1) {
            perror("snd");
            sleep(1);
        }
        else {
            printf("%s\n", send_msg);
            sleep(1);
        }
        i++;
    }
    printf("поток отправки запросов закончил работу\n");
}

void * func2() {
    printf("поток обработки ответов начал работу\n");
    while (flag_receive == 0) {
        char rcv_msg[50];
        socklen_t slen = sizeof(addr2);
        addr2.sun_family = AF_UNIX;
        strncpy(addr2.sun_path,"socket2.soc",sizeof(addr2.sun_path) - 1);
        int rv = recvfrom(server_sock_2, &rcv_msg, sizeof(rcv_msg), 0, (struct sockaddr*)&addr2, &slen);
        if (rv == -1) {
            perror("receive");
            sleep(1);
        }
        else {
            printf("%s\n", rcv_msg);
        }
    }
    printf("поток обработки ответов закончил работу\n");
}

int main() {
    printf("программа сервера работу\n");
    pthread_t id1, id2;

    server_sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    fcntl(server_sock, F_SETFL, O_NONBLOCK);
    server_sock_2 = socket(AF_UNIX, SOCK_DGRAM, 0);
    fcntl(server_sock_2, F_SETFL, O_NONBLOCK);

    addr2.sun_family = AF_UNIX;
    strncpy(addr2.sun_path,"socket2.soc",sizeof(addr2.sun_path) - 1);
    int rv = bind(server_sock_2, (struct sockaddr*)&addr2, sizeof(addr2));
    if (rv == -1) {
        perror("bind");
    }
    int optval = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR,
        &optval, sizeof(optval));

    pthread_create(&id1, NULL, func1, NULL);
    pthread_create(&id2, NULL, func2, NULL);
    printf("программа ждет нажатия клавиши\n");
    getchar();
    printf("клавиша нажата\n");
    flag_send = 1;
    flag_receive = 1;
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);

    close(server_sock);
    unlink("socket.soc");
    close(server_sock_2);
    unlink("socket2.soc");

    printf("программа сервера работу\n");
}