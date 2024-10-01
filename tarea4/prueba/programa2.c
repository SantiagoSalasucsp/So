#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>

struct msgbuf {
    long mtype;
    int signal;
    int pid;
};

void *thread_function(void *arg) {
    int msqid = ((int*)arg)[0];   // ID de la cola de mensajes
    long mtype = ((int*)arg)[1];  // Tipo de mensaje (1 o 2)
    struct msgbuf message;

    while (1) {
        // Recibir mensaje del tipo adecuado
        if (msgrcv(msqid, &message, sizeof(message) - sizeof(long), mtype, 0) < 0) {
            perror("Error al recibir mensaje");
            exit(1);
        }

        printf("Thread de tipo %ld recibió: Señal %d para PID %d\n", mtype, message.signal, message.pid);

        // Enviar la señal al proceso
        if (kill(message.pid, message.signal) < 0) {
            perror("Error al enviar la señal");
        } else {
            if (message.signal != SIGSTOP) {
                printf("Señal %d enviada al proceso %d\n", message.signal, message.pid);
            } else {
                printf("PID %d fue afectado por la señal %d (SIGSTOP)\n", message.pid, message.signal);
            }
        }
    }

    return NULL;
}

int main() {
    int msqid;
    key_t key = 1234;
    pthread_t thread1, thread2;
    int args1[2], args2[2];

    // Obtener la cola de mensajes
    if ((msqid = msgget(key, 0666)) < 0) {
        perror("Error al obtener la cola de mensajes");
        exit(1);
    }

    // Argumentos para los threads
    args1[0] = msqid;
    args1[1] = 1;  // Tipo de mensaje 1 (señales 2, 16, 17)
    args2[0] = msqid;
    args2[1] = 2;  // Tipo de mensaje 2 (señales 18, 19)

    // Crear los threads
    if (pthread_create(&thread1, NULL, thread_function, args1) != 0) {
        perror("Error al crear el thread 1");
        exit(1);
    }
    if (pthread_create(&thread2, NULL, thread_function, args2) != 0) {
        perror("Error al crear el thread 2");
        exit(1);
    }

    // Esperar a que los threads terminen (aunque nunca sucederá)
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
