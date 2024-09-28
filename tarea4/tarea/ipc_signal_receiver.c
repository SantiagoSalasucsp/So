#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum) {
    switch(signum) {
        case 2:
            printf("Recibida señal 2\n");
            break;
        case 16:
            printf("Recibida señal 16\n");
            break;
        case 17:
            printf("Recibida señal 17\n");
            break;
        case 18:
            printf("Recibida señal 18\n");
            break;
        case 19:
            printf("Recibida señal 19\n");
            break;
        default:
            printf("Recibida señal desconocida\n");
    }
}

int main() {
    // Asignar el manejador de señales
    signal(2, signal_handler);
    signal(16, signal_handler);
    signal(17, signal_handler);
    signal(18, signal_handler);
    signal(19, signal_handler);

    // Esperar infinitamente para recibir señales
    while (1) {
        pause();  // Pausar el proceso hasta que llegue una señal
    }

    return 0;
}
