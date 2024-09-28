#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

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

// Función principal
int main() {
    int msqid;
    key_t key;
    struct msgbuf sbuf;

    key = 1234;

    // Crear la cola de mensajes
    msqid = msgget(key, IPC_CREAT | 0666);
    if (msqid < 0) {
        die("msgget");
    }

    int signal_choice;
    pid_t pid;

    while (1) {
        // Menú para elegir señal
        printf("Elige una señal a enviar:\n");
        printf("1. Señal 2\n");
        printf("2. Señal 16\n");
        printf("3. Señal 17\n");
        printf("4. Señal 18\n");
        printf("5. Señal 19\n");
        printf("6. Salir\n");
        printf("Opción: ");
        scanf("%d", &signal_choice);
        
        if (signal_choice == 6) {
            break;
        }

        printf("Ingresa el PID del proceso al que deseas enviar la señal: ");
        scanf("%d", &pid);

        // Llenar el buffer de mensaje
        sbuf.mtype = 1;  // Tipo de mensaje
        sbuf.signal = signal_choice + 1;  // Ajustar el número de la señal
        sbuf.pid = pid;

        // Enviar mensaje
        if (msgsnd(msqid, &sbuf, sizeof(int) + sizeof(pid_t), IPC_NOWAIT) < 0) {
            die("msgsnd");
        }
    }

    // Limpiar recursos antes de salir
    msgctl(msqid, IPC_RMID, NULL);  // Eliminar cola de mensajes

    return 0;
}
