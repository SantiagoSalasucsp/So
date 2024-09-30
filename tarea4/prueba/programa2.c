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
        if (message.signal == 19) {
            printf("PID %d fue afectado por la señal 19 (SIGSTOP)\n", message.pid);
        }

        // Enviar la señal al proceso
        if (kill(message.pid, message.signal) < 0) {
            perror("kill");
        } else {
            if (message.signal != 19) {
                printf("Señal %d enviada al proceso %d\n", message.signal, message.pid);
            }
        }
    }

    return NULL;
}

// ... (el resto del código permanece igual)