#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int fd;
 

    
    pid = fork();
    if (pid < 0) {
        perror("Error en fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        

        
        fd = open("xd.txt", O_RDONLY);
        if (fd < 0) {
            perror("Error al abrir xd.txt para lectura");
            exit(EXIT_FAILURE);
        }

        
        dup2(fd, 0);
        close(fd);

        char *args[] = {"./pp", NULL};
        execv(args[0], args);

        perror("Error en execv");
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
    }

    return 0;
}
