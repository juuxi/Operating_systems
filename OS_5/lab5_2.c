#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>

int main () {
    printf("Программа 2 начала работу\n");
    sem_t* sem = sem_open("/my_sem", O_CREAT, 0644, 1);
    int file_dis;
    file_dis = open("./lab5.txt", O_CREAT|O_WRONLY|O_APPEND);
    fd_set rfds;
    FD_SET(0, &rfds);
    struct timeval timeout;
    timeout.tv_sec = 1;
    while (1) {
        sem_wait(sem);
        for (int i = 0; i < 10; i++) { //Вход в КУ
            char buf = '2';
            if (write(file_dis, &buf, sizeof(char)) == -1)
                perror("");
            printf("2");
            fflush(stdout);
            sleep(1);
        } //Выход из КУ
        sem_post(sem);
        sleep(1);
        printf("Программа 2 ожидает нажатия клавиши\n");
        if (select(1, &rfds, NULL, NULL, &timeout) != 0) {
            printf("Клавиша в программе 2 нажата\n");
            break;
        } else {
            FD_SET(0, &rfds);
        }
    }
    close(file_dis);
    sem_close(sem);
    sem_unlink("/my_sem");
    printf("Программа 2 завершила работу\n");
}