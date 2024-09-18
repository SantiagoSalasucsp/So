#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <signal> <pid>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sig = atoi(argv[1]);
    pid_t pid = atoi(argv[2]);

    if (kill(pid, sig) == -1) {
        perror("kill");
        exit(EXIT_FAILURE); 
    }

    printf("Señal %d enviada al proceso %d\n", sig, pid);
    return 0;
}
//gcc mykill.c -o mykill
//sleep 999 &
//./mykill 2 1234