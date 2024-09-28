#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>  // Para la función kill
#include <pthread.h>  // Para el manejo de hilos

#define MAXSIZE 128

struct msgbuf {
    long mtype;
    int signal;  // Señal a enviar
    pid_t pid;   // PID del proceso destino
};

// Función para manejar errores
void die(char *s) {
    perror(s);
    exit(1);
}

// Hilo para manejar señales de tipo 1
void *hilo_tipo_1(void *arg) {
    int msqid;
    key_t key;
    struct msgbuf rcvbuffer;

    key = 1234;

    // Obtener el ID de la cola de mensajes
    if ((msqid = msgget(key, 0666)) < 0) {
        die("msgget");
    }

    while (1) {
        // Recibir mensaje de la cola
        if (msgrcv(msqid, &rcvbuffer, sizeof(int) + sizeof(pid_t), 1, 0) < 0) {
            die("msgrcv");
        }

        // Manejar señales de tipo 1
        if (rcvbuffer.signal == 2 || rcvbuffer.signal == 16 || rcvbuffer.signal == 17) {
            printf("Enviando señal %d al proceso con PID %d\n", rcvbuffer.signal, rcvbuffer.pid);
            kill(rcvbuffer.pid, rcvbuffer.signal);  // Enviar la señal al proceso
        }
    }
    return NULL;
}

// Hilo para manejar señales de tipo 2
void *hilo_tipo_2(void *arg) {
    int msqid;
    key_t key;
    struct msgbuf rcvbuffer;

    key = 1234;

    // Obtener el ID de la cola de mensajes
    if ((msqid = msgget(key, 0666)) < 0) {
        die("msgget");
    }

    while (1) {
        // Recibir mensaje de la cola
        if (msgrcv(msqid, &rcvbuffer, sizeof(int) + sizeof(pid_t), 1, 0) < 0) {
            die("msgrcv");
        }

        // Manejar señales de tipo 2
        if (rcvbuffer.signal == 18 || rcvbuffer.signal == 19) {
            printf("Enviando señal %d al proceso con PID %d\n", rcvbuffer.signal, rcvbuffer.pid);
            kill(rcvbuffer.pid, rcvbuffer.signal);  // Enviar la señal al proceso
        }
    }
    return NULL;
}

// Función principal
int main() {
    pthread_t thread1, thread2;

    // Crear los hilos
    pthread_create(&thread1, NULL, hilo_tipo_1, NULL);
    pthread_create(&thread2, NULL, hilo_tipo_2, NULL);

    // Esperar que los hilos terminen
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
