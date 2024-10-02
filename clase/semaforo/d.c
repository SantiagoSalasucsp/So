#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define KEY 0X1111
union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};
struct sembuf p={0,-1,SEM_UNDO};
struct sembuf v={0,+1,SEM_UNDO};

int main(){
    int id=semget(KEY,1,0666 |IPC_CREAT);
    union semun u;
    
    u.val=1;
    semctl(id,0,SETVAL,u);
    int d;
    while(1){
        int k=0;
        k=scanf(d);
        if(k!=0){
            
            break;
        }
    }
    
    semop(id,&p,1);
    int f=0;
    int g=0;
    while(f<100){

        g=0;
        while(g<10000){
            g++;
        }
        printf("a ver");
        g=0;
        while(g<10000){
            g++;
        }
    }
    semop(id,&v,1);

    return 0;
}