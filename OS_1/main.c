#include <stdio.h>
#include <pthread.h>

void * func1(void * arg) {
    printf("поток 1 начал работу\n");
    int* flag1 = (int*)arg;
    while(*flag1 == 0) {
        putchar('1');
        fflush(stdout);
        sleep(1);
    }
    printf("поток 1 закончил работу\n");
    pthread_exit((void*)1);
}

void * func2(void * arg) {
    printf("поток 2 начал работу\n");
    int* flag2 = (int*)arg;
    while(*flag2 == 0) {
        putchar('2');
        fflush(stdout);
        sleep(1);
    }
    printf("поток 2 закончил работу\n");
    pthread_exit((void*)2);
}

void main() {
    printf("программа начала работу\n");
    int flag1 = 0;
    int flag2 = 0;
    int exitcode1, exitcode2;
    pthread_t id1, id2;
    pthread_create(&id1, NULL, func1, &flag1);
    pthread_create(&id2, NULL, func2, &flag2);
    printf("программа ждет нажатия клавиши\n");
    getchar();
    printf("клавиша нажата\n");
    flag1 = 1;
    flag2 = 1;
    pthread_join(id1, (void**)&exitcode1);
    pthread_join(id2, (void**)&exitcode2);
    printf("first exitcode = %d\n", exitcode1);
    printf("second exitcode = %d\n", exitcode2);
    printf("\n");
    printf("Стандартные атрибуты pthread:\n");
    printf("Detach state        = PTHREAD_CREATE_JOINABLE - поток зависит от своего родителя и может завершаться функцией pthread_join() \n");
    printf("Scope               = PTHREAD_SCOPE_SYSTEM - поток разделяет ресурсы компьютера с остальными потоками по общим правилам, при нестандартном значении PTHREAD_SCOPE_PROCESS он находится в одном процессе с остальными потоками созданными с той же установкой\n");
    printf("Inherit scheduler   = PTHREAD_INHERIT_SCHED - атрибуты планирования берутся у порождающего потока\n");
    printf("Scheduling policy   = SCHED_OTHER - стандартное для Linux планирование с разделением времени\n");
    printf("Scheduling priority = 0 - приоритет потока (чем он выше, тем больше шанс, что ему предоставят тот или иной ресурс компьютера) \n");
    printf("Guard size          = 4096 bytes - определяет число байт, которые будут зарезервированы в конце стека данного потока чтобы действовать как \"защищенная зона\" для стека. Стандартный размер равен стандартному размеру страницы в системе\n");
    printf("Stack address       = 0x40196000 - адрес места расположения стека потока\n");
    printf("Stack size          = 0x201000 bytes - размер этого стека\n");
    printf("\n");
    printf("программа завершила работу\n");
}