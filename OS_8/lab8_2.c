#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/queue.h>
#include <sys/stat.h>
#include <ctype.h>

int flag_send = 0;
int flag_receive = 0;
int server_sock_2;
struct sockaddr_un addr;

void * func1() {
    printf("поток отправки запросов начал работу\n");
    char send_msg[50];
    int i = 1;
    while(flag_send == 0) {
        sprintf(send_msg, "Сообщение %d отправлено", i);
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path,"socket.soc",sizeof(addr.sun_path) - 1);
        addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';
        int rv = sendto(server_sock_2, send_msg, strlen(send_msg) + 1, 0, (struct sockaddr*)&addr, sizeof(addr));
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
        socklen_t slen = sizeof(addr);
        int rv = recvfrom(server_sock_2, rcv_msg, sizeof(rcv_msg), 0, (struct sockaddr*)&addr, &slen);
        if (rv == -1) {
            perror("receive");
            sleep(1);
        }
        else {
            int i = 0;
            long val = 0;
            char* iter = rcv_msg;
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
            printf("Ответ на сообщение %d принят: %ld\n", i, val);
        }
       sleep(1);
    }
    printf("поток обработки ответов закончил работу\n");
}

int main() {
    printf("программа клиента начала работу\n");
    pthread_t id1, id2;

    server_sock_2 = socket(AF_UNIX, SOCK_DGRAM, 0);
    fcntl(server_sock_2, F_SETFL, O_NONBLOCK);

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path,"socket2.soc",sizeof(addr.sun_path) - 1);
    int rv = bind(server_sock_2, (struct sockaddr*)&addr, sizeof(addr));
    if (rv == -1) {
        perror("bind");
    }
    int optval = 1;
    setsockopt(server_sock_2, SOL_SOCKET, SO_REUSEADDR,
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

    close(server_sock_2);
    unlink("socket.soc");
    unlink("socket2.soc");

    printf("программа клиента закончила работу\n");
}