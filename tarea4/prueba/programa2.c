#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <csignal>
#include <thread>
#define MAXSIZE     128

void die(char *s)
{
  perror(s);
  exit(1);
}

struct my_msgbuf
{
    long    mtype;
    char    mtext[MAXSIZE];
};

int msqid;

void recibirTipo1(){
	struct my_msgbuf rcvbuffer;
	int numsig;
	int pid;

	while(true){
		if(msgrcv(msqid, &rcvbuffer, MAXSIZE,1,0)<0){
			die("msgrcv tipo 1");
		}

		sscanf(rcvbuffer.mtext, "%d %d", &numsig, &pid);

		// Imprimir mensaje antes de enviar señal
		std::cout << "Thread tipo 1 enviando señal " << numsig << " al proceso con PID " << pid << "\n";

		// Enviar la señal
		kill(pid, numsig);
	}
}

void recibirTipo2(){
	struct my_msgbuf rcvbuffer;
	int numsig;
	int pid;

	while(true){
		if(msgrcv(msqid, &rcvbuffer, MAXSIZE,2,0)<0){
			die("msgrcv tipo 2");
		}

		sscanf(rcvbuffer.mtext, "%d %d", &numsig, &pid);

		// Imprimir mensaje antes de enviar señal
		if (numsig == SIGSTOP) {
			std::cout << "El PID " << pid << " será afectado por SIGSTOP (19)\n";
		} else if (numsig == SIGCONT) {
			std::cout << "El PID " << pid << " será reanudado con SIGCONT (18)\n";
		} else {
			std::cout << "Thread tipo 2 enviando señal " << numsig << " al proceso con PID " << pid << "\n";
		}

		// Enviar la señal
		kill(pid, numsig);
	}
}

int main()
{
    key_t key;
    key = 1234;

    if ((msqid = msgget(key, 0666)) < 0)  die("msgget()");

    std::thread thread1(recibirTipo1);
    std::thread thread2(recibirTipo2);

    thread1.join();
    thread2.join();

    exit(0);
}
