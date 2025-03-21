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


int flag_rcv = 0;
int flag_process = 0;
int client_sock;
int client_sock_2;
struct sockaddr_un addr;
struct sockaddr_un addr2;
pthread_mutex_t mutex;

struct entry {
    char* msg;
    STAILQ_ENTRY(entry) entries;
};
struct stailhead head;
STAILQ_HEAD(stailhead, entry);

void * func1() {
    printf("поток приема начал работу\n");
    while(flag_rcv == 0) {
        char rcv_msg[50];
        socklen_t slen = sizeof(addr2);
        int rv = recvfrom(client_sock, rcv_msg, sizeof(rcv_msg), 0, (struct sockaddr*)&addr2, &slen);
        if (rv == -1) {
            perror("receive");
            sleep(1);
        }
        else {
            pthread_mutex_lock(&mutex);
            struct entry *ent_1;
            ent_1 = malloc(sizeof(struct entry));
            ent_1->msg = rcv_msg;
            STAILQ_INSERT_TAIL(&head, ent_1, entries);
            pthread_mutex_unlock(&mutex);
        }
    }
    printf("поток приема закончил работу\n");
}

void * func2() {
    printf("поток обработки начал работу\n");
    while(flag_process == 0) {
        int i = 0;
        pthread_mutex_lock(&mutex);
        if (!STAILQ_EMPTY(&head)) {
            struct entry *first_ent;
            first_ent = STAILQ_FIRST(&head);
            STAILQ_REMOVE_HEAD(&head, entries);
            pthread_mutex_unlock(&mutex);
            char* iter = first_ent->msg;
            while (!isdigit(*iter)) {
                iter++;
            }
            while (isdigit(*iter)) {
                i = i*10+*iter-'0';
                iter++;
            }
            printf("Сообщение %d принято\n", i);
            free(first_ent);
            long func_res = pathconf("./", _PC_NAME_MAX);
            char send_msg[50];
            sprintf(send_msg, "Ответ на сообщение %d: %ld", i, func_res);
            int rv = sendto(client_sock, send_msg, strlen(send_msg)+1, 0, (struct sockaddr*)&addr2, sizeof(addr2));
            if (rv == -1) {
                perror("send");
            }
            else {
                printf("Ответ на сообщение %d отправлен: %ld\n", i, func_res);
            }
        }
        else {
            pthread_mutex_unlock(&mutex);
        }
    }
    printf("поток обработки закончил работу\n");
}

int main() {
    printf("программа сервера начала работу\n");
    pthread_t id1, id2;
    pthread_mutex_init(&mutex, NULL);

    client_sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    fcntl(client_sock, F_SETFL, O_NONBLOCK);

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path,"socket.soc",sizeof(addr.sun_path) - 1);
    int rv = bind(client_sock, (struct sockaddr*)&addr, sizeof(addr));
    if (rv == -1) {
        perror("bind");
    }

    STAILQ_INIT(&head);

    pthread_create(&id1, NULL, func1, NULL);
    pthread_create(&id2, NULL, func2, NULL);
    printf("программа ждет нажатия клавиши\n");
    getchar();
    printf("клавиша нажата\n");
    flag_rcv = 1;
    flag_process = 1;
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);

    close(client_sock);
    close(client_sock_2);
    unlink("socket.soc");
    unlink("socket2.soc");

    printf("программа сервера закончила работу\n");
}