#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>

int main () {
    /* sem_t* sem = sem_open("/my_sem", 0); */
    int file_dis;
    file_dis = open("./lab5.txt", O_CREAT);
    fd_set rfds;
    FD_SET(0, &rfds);
    struct timeval timeout;
    timeout.tv_sec = 1;
    while (1) {
        for (int i = 0; i < 10; i++) { //Вход в КУ
            char buf = '1';
            write(file_dis, &buf, sizeof(char));
            printf("1");
            fflush(stdout);
            sleep(1);
        } //Выход из КУ
        sleep(1);
        if (select(1, &rfds, NULL, NULL, &timeout) == 1) {
            break;
        }
    }
    close(file_dis);
}