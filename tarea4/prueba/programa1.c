#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAXSIZE 128

struct msgbuf {
    long mtype;
    int signal;
    int pid;
};

int main() {
    int msqid;
    key_t key = 1234;
    struct msgbuf message;
    int option, pid;

    // Crear o obtener la cola de mensajes
    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    while(1) {
        // Mostrar el menú y pedir datos
        printf("\nMenú de señales:\n");
        printf("1. Señal 2 (SIGINT)\n");
        printf("2. Señal 16 (SIGSTKFLT)\n");
        printf("3. Señal 17 (SIGCHLD)\n");
        printf("4. Señal 18 (SIGCONT)\n");
        printf("5. Señal 19 (SIGSTOP)\n");
        printf("6. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &option);

        if (option == 6) break;

        if (option < 1 || option > 5) {
            printf("Opción inválida. Intente de nuevo.\n");
            continue;
        }

        printf("Ingrese el PID del proceso: ");
        scanf("%d", &pid);

        // Preparar el mensaje
        message.mtype = (option <= 3) ? 1 : 2;  // Tipo 1 para 2,16,17; Tipo 2 para 18,19
        message.signal = (option == 1) ? 2 : (option == 2) ? 16 : (option == 3) ? 17 : (option == 4) ? 18 : 19;
        message.pid = pid;

        // Enviar el mensaje a la cola
        if (msgsnd(msqid, &message, sizeof(message) - sizeof(long), 0) < 0) {
            perror("msgsnd");
            exit(1);
        }

        printf("Mensaje enviado: Señal %d para PID %d\n", message.signal, message.pid);
    }

    return 0;
}