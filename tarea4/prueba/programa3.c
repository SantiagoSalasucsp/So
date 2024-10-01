#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum) {
    printf("PID %d fue afectado por la señal %d\n", getpid(), signum);
}

int main() {
    // Registrar los manejadores de señales
    signal(SIGINT, signal_handler);   // 2
    signal(SIGSTKFLT, signal_handler);// 16
    signal(SIGCHLD, signal_handler);  // 17
    signal(SIGCONT, signal_handler);  // 18
    // No podemos manejar SIGSTOP (19)

    printf("Proceso receptor iniciado. PID: %d\n", getpid());

    // Bucle infinito para mantener el proceso en ejecución
    while (1) {
        pause();  // Esperar por señales
    }

    return 0;
}
