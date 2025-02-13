#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int flag1 = 0;
int flag2 = 0;

void * func1() {
    printf("поток 1 начал работу\n");
    while(flag1 == 0) {
        for (int i = 0; i < 10; i++) { //вход в КУ
            putchar('1');
            fflush(stdout);
            sleep(1);
        } //выход из КУ
        sleep(1);
    }
    printf("поток 1 закончил работу\n");
}

void * func2() {
    printf("поток 2 начал работу\n");
    while(flag2 == 0) {
        for (int i = 0; i < 10; i++) { //вход в КУ
            putchar('2');
            fflush(stdout);
            sleep(1);
        } //выход из КУ
        sleep(1);
    }
    printf("поток 2 закончил работу\n");
}

void main() {
    printf("программа начала работу\n");
    pthread_t id1, id2;
    pthread_create(&id1, NULL, func1, NULL);
    pthread_create(&id2, NULL, func2, NULL);
    printf("программа ждет нажатия клавиши\n");
    getchar();
    printf("клавиша нажата\n");
    flag1 = 1;
    flag2 = 1;
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
    printf("программа завершила работу\n");
}