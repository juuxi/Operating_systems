#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
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
    char buf[sizeof(long)];
    while(flag1 == 0) { //error msg too long keeps occuring even though currently 1 byte is sent
        unsigned prio;
        int result = mq_receive(queue, buf, sizeof(char), &prio);
        if (result > 0) {
            printf("%s\n", buf);
        }
        else if (result == -1){
            perror("receive");
            sleep(1);
        }
    }
    printf("поток закончил работу\n");
}

int main() {
    printf("программа 2 начала работу\n");
    pthread_t id1;

    queue = mq_open("/my_q", O_CREAT|O_RDONLY|O_NONBLOCK, 0644, NULL);
    struct mq_attr queue_attr;
    mq_getattr(queue, &queue_attr);
    struct mq_attr new_queue_attr = queue_attr;
    new_queue_attr.mq_msgsize = 16;
    mq_setattr(queue, &new_queue_attr, &queue_attr);

    pthread_create(&id1, NULL, func1, NULL);
    printf("программа ждет нажатия клавиши\n");
    getchar();
    printf("клавиша нажата\n");
    flag1 = 1;
    pthread_join(id1, NULL);

    mq_close(queue);
    mq_unlink("/my_q");
    printf("программа 2 завершила работу\n");
}