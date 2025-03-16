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
int server_sock;
struct sockaddr_un addr;

void * func1() {
    printf("поток начал работу\n");
    char send_msg[50];
    int i = 1;
    while(flag1 == 0) {
        long func_res = pathconf("./", _PC_NAME_MAX);
        sprintf(send_msg, "Сообщение %d отправлено: %ld", i, func_res);
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
    printf("поток закончил работу\n");
}

int main() {
    printf("программа сервера работу\n");
    pthread_t id1;

    server_sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    fcntl(server_sock, F_SETFL, O_NONBLOCK);
    /* addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path,"socket.soc",sizeof(addr.sun_path) - 1);
    int rv = bind(server_sock, (struct sockaddr*)&addr, sizeof(addr));
    if (rv == -1) {
        perror("bind");
    }
    int optval = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR,
        &optval, sizeof(optval)); */

    pthread_create(&id1, NULL, func1, NULL);
    printf("программа ждет нажатия клавиши\n");
    getchar();
    printf("клавиша нажата\n");
    flag1 = 1;
    pthread_join(id1, NULL);

    close(server_sock);
    unlink("socket.soc");

    printf("программа сервера работу\n");
}