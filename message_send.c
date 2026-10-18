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

int main()
{
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

    sbuf.mtype = 1;
    (void)strcpy(sbuf.mtext, "Are you hearing me?");
    buf_length = strlen(sbuf.mtext) + 1;

    if ( msgsnd( msqid, &sbuf, buf_length, IPC_NOWAIT) < 0)
    {
        perror("msgsnd");
        exit(1);
    }

    printf("Proceso 1 envia: %s\n", sbuf.mtext);

    // Esperar la respuesta del proceso 2
    if (msgrcv(msqid, &rbuf, MSGSZ, 2, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }

    printf("Proceso 1 recibe: %s\n", rbuf.mtext);

    // Mostrar la respuesta final solicitada
    printf("Proceso 1 responde: I can hear you too\n");
    
    exit(0);
}