#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MSGSZ 128

typedef struct msgbuf{
    long mtype;
    char mtext[MSGSZ];
} message_buf;


int main(){
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    message_buf rbuf, sbuf;
    size_t buf_length;

    key = 1234;

    if((msqid = msgget(key, msgflg)) < 0){
        perror("msgget()");
        exit(1);
    }


    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0){
        perror("msgrcv");
        exit(1);
    }

    printf("Proceso 2 recibe: %s\n", rbuf.mtext);

    // Preparar la respuesta
    sbuf.mtype = 2;
    strcpy(sbuf.mtext, "Loud and clear");
    buf_length = strlen(sbuf.mtext) + 1;

    // Enviar la respuesta al proceso 1
    if (msgsnd(msqid, &sbuf, buf_length, 0) < 0) {
        perror("msgsnd");
        exit(1);
    }

    printf("Proceso 2 responde: %s\n", sbuf.mtext);

    exit(1);
}