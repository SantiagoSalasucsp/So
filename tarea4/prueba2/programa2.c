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
    int msqid;
    struct message msg;
    long mtype = (long)arg;

    if ((msqid = msgget(KEY, 0666)) < 0) {
        perror("msgget");
        return NULL;
    }

    while(1) {
        if (msgrcv(msqid, &msg, sizeof(struct message) - sizeof(long), mtype, 0) == -1) {
            perror("msgrcv");
            sleep(1);
            continue;
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
    pthread_t thread1, thread2;
    long thread1_type = 1;  // Para señales 2, 16, 17
    long thread2_type = 2;  // Para señales 18, 19

    if (pthread_create(&thread1, NULL, thread_function, (void *)thread1_type) != 0) {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_create(&thread2, NULL, thread_function, (void *)thread2_type) != 0) {
        perror("pthread_create");
        exit(1);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}