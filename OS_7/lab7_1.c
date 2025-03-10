#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <mqueue.h>
#include <sys/stat.h>

int flag1 = 0;
mqd_t queue;

void * func1() {
    printf("поток начал работу\n");
    long buf;
    while(flag1 == 0) {
        buf = pathconf("./", _PC_NAME_MAX);
        printf("%ld\n", buf);
        char msg[sizeof(long)];
        sprintf(msg, "%ld", buf);
        int result = mq_send(queue, msg, sizeof(long), 1);
        if (result == -1) {
            perror("send");
        }
        sleep(1);
    }
    printf("поток закончил работу\n");
}

int main() {
    printf("программа 1 начала работу\n");
    pthread_t id1;
    struct mq_attr attr;  
    attr.mq_flags = 0;  
    attr.mq_maxmsg = 10;  
    attr.mq_msgsize = 16;  
    attr.mq_curmsgs = 0; 

    queue = mq_open("/my_q", O_CREAT|O_WRONLY|O_NONBLOCK, 0644, &attr);

    pthread_create(&id1, NULL, func1, NULL);
    printf("программа ждет нажатия клавиши\n");
    getchar();
    printf("клавиша нажата\n");
    flag1 = 1;
    pthread_join(id1, NULL);

    mq_close(queue);
    mq_unlink("/my_q");
    printf("программа 1 завершила работу\n");
}