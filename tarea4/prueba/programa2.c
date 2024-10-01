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
    int msqid = *(int*)arg;
    struct msgbuf message;
    long mtype = (long)pthread_self() % 2 + 1;  // 1 para el primer thread, 2 para el segundo

    while(1) {
        // Esperar y recibir mensaje de la cola
        if (msgrcv(msqid, &message, sizeof(message) - sizeof(long), mtype, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }

        printf("Thread %ld recibió: Señal %d para PID %d\n", mtype, message.signal, message.pid);

        // Manejar el caso especial de SIGSTOP (19)
        if (message.signal == SIGSTOP) {
            printf("PID %d fue afectado por la señal 19 (SIGSTOP)\n", message.pid);
        }

        // Enviar la señal al proceso
        if (kill(message.pid, message.signal) < 0) {
            perror("kill");
        } else {
            if (message.signal != SIGSTOP) {
                printf("Señal %d enviada al proceso %d\n", message.signal, message.pid);
            }
        }
    }

    return NULL;
}

int main() {
    int msqid;
    key_t key = 1234;
    pthread_t thread1, thread2;

    // Obtener la cola de mensajes
    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    // Crear los threads
    if (pthread_create(&thread1, NULL, thread_function, &msqid) != 0) {
        perror("pthread_create");
        exit(1);
    }
    if (pthread_create(&thread2, NULL, thread_function, &msqid) != 0) {
        perror("pthread_create");
        exit(1);
    }

    // Esperar a que los threads terminen (lo cual nunca sucederá en este caso)
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}