#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum) {
    printf("PID %d fue afectado por la señal %d\n", getpid(), signum);
}

int main() {
    printf("Proceso receptor iniciado. PID: %d\n", getpid());

    signal(SIGINT, signal_handler);
    signal(SIGSTKFLT, signal_handler);
    signal(SIGCHLD, signal_handler);
    signal(SIGCONT, signal_handler);
    signal(SIGSTOP, signal_handler);

    while(1) {
        sleep(1);
    }

    return 0;
}