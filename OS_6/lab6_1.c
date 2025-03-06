#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int flag1 = 0;
sem_t* write_sem;
sem_t* read_sem;
int shm_id;
char* addr;
long local_v;
int file;

void * func1() {
    printf("поток начал работу\n");
    long local_v;
    while(flag1 == 0) {
        local_v = pathconf("./", _PC_NAME_MAX);
        printf("%ld\n", local_v);
        memcpy(addr, &local_v, sizeof(long));
        sem_post(write_sem);
        sem_wait(read_sem);
        sleep(1);
    }
    printf("поток закончил работу\n");
}

void sig_handler() {
    sem_close(read_sem);
    sem_close(write_sem);
    sem_unlink("/write_sem");
    sem_unlink("/read_sem");
    shmdt(addr);
    shmctl(shm_id, IPC_RMID, NULL);
    exit(0);
}

int main(int argc, char* argv[]) {
    printf("программа 1 начала работу\n");
    signal(SIGINT, sig_handler);
    file = open("./lab6.txt", O_CREAT);
    close(file);
    pthread_t id1;
    key_t shm_key = ftok("./lab6.txt", argv[optind+1][0]);
    if (shm_key == -1) {
        perror("ftok");
    }
    shm_id = shmget(shm_key, 4096, IPC_CREAT|0666);
    addr = shmat(shm_id, NULL, 0);
    write_sem = sem_open("/write_sem", O_CREAT, 0644, 0);
    read_sem = sem_open("/read_sem", O_CREAT, 0644, 0);
    pthread_create(&id1, NULL, func1, NULL);
    printf("программа ждет нажатия клавиши\n");
    getchar();
    printf("клавиша нажата\n");
    flag1 = 1;
    pthread_join(id1, NULL);

    sem_close(read_sem);
    sem_close(write_sem);
    sem_unlink("/write_sem");
    sem_unlink("/read_sem");
    shmdt(addr);
    shmctl(shm_id, IPC_RMID, NULL);
    printf("программа 1 закончила работу\n");
}