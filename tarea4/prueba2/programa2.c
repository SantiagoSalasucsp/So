#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <signal.h>
#include <unistd.h>

#define KEY 1234

struct message {
    long mtype;
    int signal;
    int pid;
};

void *thread_function(void *arg) {
    int msqid = *(int*)arg;
    struct message msg;
    long mtype = (long)pthread_self() % 2 + 1;

    while(1) {
        if (msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), mtype, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }

        printf("Thread de tipo %ld recibió: Señal %d para PID %d\n", mtype, msg.signal, msg.pid);

        if (kill(msg.pid, 0) == 0) {
            if (kill(msg.pid, msg.signal) < 0) {
                perror("kill");
            } else {
                printf("Señal %d enviada al proceso %d\n", msg.signal, msg.pid);
            }
        } else {
            printf("Proceso con PID %d no existe o ya ha terminado.\n", msg.pid);
        }
    }
    return NULL;
}

int main() {
    int msqid;
    pthread_t thread1, thread2;

    if ((msqid = msgget(KEY, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    if (pthread_create(&thread1, NULL, thread_function, &msqid) != 0 ||
        pthread_create(&thread2, NULL, thread_function, &msqid) != 0) {
        perror("pthread_create");
        exit(1);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}