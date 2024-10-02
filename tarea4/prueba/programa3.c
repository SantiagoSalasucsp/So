#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
/*
void signal_handler(int signum) {
    printf("PID %d fue afectado por la señal %d\n", getpid(), signum);
}*/

void signal_handler(int signum) {
    if (signum == SIGCONT) {
        printf("Recibida señal SIGCONT. PID %d fue reanudado.\n", getpid());
    } else {
        printf("PID %d fue afectado por la señal %d\n", getpid(), signum);
    }
    fflush(stdout);  // Asegúrate de que el mensaje se imprima de inmediato
}


int main() {
    
    signal(SIGINT, signal_handler);   // 2
    signal(SIGSTKFLT, signal_handler);// 16
    signal(SIGCHLD, signal_handler);  // 17
    signal(SIGCONT, signal_handler);  // 18
    

    printf("Proceso receptor iniciado. PID: %d\n", getpid());

   
    while (1) {
        pause();  
    }

    return 0;
}
