#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>

#define MAXSIZE 128

struct msgbuf {
    long mtype;
    int signal;
    int pid;
};

void *thread_function(void *arg) {
    int thread_type = *(int*)arg;
    int msqid;
    key_t key = 1234;
    struct msgbuf message;

    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    while (1) {
        if (msgrcv(msqid, &message, sizeof(message) - sizeof(long), thread_type, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }

        printf("Thread de tipo %d recibió: Señal %d para PID %d\n", thread_type, message.signal, message.pid);

        if (kill(message.pid, 0) == 0) {
            if (kill(message.pid, message.signal) < 0) {
                perror("Error al enviar la señal");
            } else {
                printf("Señal %d enviada al proceso %d\n", message.signal, message.pid);
            }
        } else {
            printf("Proceso con PID %d no existe o ya ha terminado.\n", message.pid);
        }
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int thread1_type = 1;  // Para señales 2, 16, 17
    int thread2_type = 2;  // Para señales 18, 19

    if (pthread_create(&thread1, NULL, thread_function, &thread1_type) != 0) {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_create(&thread2, NULL, thread_function, &thread2_type) != 0) {
        perror("pthread_create");
        exit(1);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}