#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

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

    
    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("Error al crear la cola de mensajes");
        exit(1);
    }

    while (1) {
        
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

        
        if (kill(pid, 0) < 0) {
            perror("Error: PID inválido");
            continue; 
        }

        
        message.mtype = (option <= 3) ? 1 : 2;  
        message.signal = (option == 1) ? SIGINT : (option == 2) ? SIGSTKFLT : (option == 3) ? SIGCHLD : (option == 4) ? SIGCONT : SIGSTOP;
        message.pid = pid;

       
        if (msgsnd(msqid, &message, sizeof(message) - sizeof(long), 0) < 0) {
            perror("Error al enviar el mensaje");
            exit(1);
        }

        printf("Mensaje enviado: Señal %d para PID %d\n", message.signal, message.pid);
    }

    return 0;
}
