#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#define KEY 1234

struct message {
    long mtype;
    int signal;
    int pid;
};

void *thread_function(void *arg) {
    int msqid = *(int*)arg;
    struct message msg;
    long mtype = (long)arg == 1 ? 1 : 2;  // Thread 1 maneja tipo 1, Thread 2 maneja tipo 2

    while(1) {
        if (msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), mtype, 0) < 0) {
            perror("msgrcv");
            continue;
        }

        printf("Thread de tipo %ld recibió: Señal %d para PID %d\n", mtype, msg.signal, msg.pid);

        if (kill(msg.pid, 0) == 0) {
            if (msg.signal == SIGSTOP || msg.signal == SIGCONT) {
                // Para SIGSTOP y SIGCONT, usamos kill() directamente
                if (kill(msg.pid, msg.signal) < 0) {
                    perror("kill (SIGSTOP/SIGCONT)");
                } else {
                    printf("Señal %d enviada al proceso %d\n", msg.signal, msg.pid);
                }
            } else {
                // Para otras señales, usamos pthread_kill() para mayor compatibilidad
                if (pthread_kill(msg.pid, msg.signal) != 0) {
                    perror("pthread_kill");
                } else {
                    printf("Señal %d enviada al proceso %d\n", msg.signal, msg.pid);
                }
            }
        } else {
            if (errno == ESRCH) {
                printf("Proceso con PID %d no existe.\n", msg.pid);
            } else {
                perror("kill");
            }
        }
    }
    return NULL;
}

int main() {
    int msqid;
    pthread_t thread1, thread2;
    int thread1_type = 1;
    int thread2_type = 2;

    if ((msqid = msgget(KEY, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    if (pthread_create(&thread1, NULL, thread_function, &thread1_type) != 0 ||
        pthread_create(&thread2, NULL, thread_function, &thread2_type) != 0) {
        perror("pthread_create");
        exit(1);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}