#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#define KEY 1234

struct message {
    long mtype;
    int signal;
    int pid;
};

int main() {
    int msqid;
    struct message msg;
    int option, pid;

    if ((msqid = msgget(KEY, IPC_CREAT | 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    while(1) {
        printf("\nMenú de señales:\n");
        printf("1. Señal 2 (SIGINT)\n2. Señal 16 (SIGSTKFLT)\n3. Señal 17 (SIGCHLD)\n");
        printf("4. Señal 18 (SIGCONT)\n5. Señal 19 (SIGSTOP)\n6. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &option);

        if (option == 6) break;

        printf("Ingrese el PID del proceso: ");
        scanf("%d", &pid);

        msg.signal = option == 1 ? 2 : (option == 2 ? 16 : (option == 3 ? 17 : (option == 4 ? 18 : 19)));
        msg.pid = pid;
        msg.mtype = (msg.signal == 18 || msg.signal == 19) ? 2 : 1;

        if (msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0) < 0) {
            perror("msgsnd");
            exit(1);
        }

        printf("Mensaje enviado: Señal %d para PID %d\n", msg.signal, msg.pid);
    }

    return 0;
}