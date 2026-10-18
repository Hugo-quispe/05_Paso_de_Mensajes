#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128

typedef struct msgbuf {
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

    printf("Servidor iniciado. Esperando solicitudes...\n");

    while (1) {

        // Recibir una solicitud del cliente 1 o del cliente 2
        if (msgrcv(msqid, &rbuf, MSGSZ, -2, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }

        printf("Servidor recibe del tipo %ld: %s\n",
               rbuf.mtype, rbuf.mtext);

        // Preparar la respuesta segun el cliente
        if (rbuf.mtype == 1) {
            sbuf.mtype = 3;
            snprintf(sbuf.mtext, MSGSZ,
                     "Respuesta privada para cliente 1: solicitud recibida");
        }
        else if (rbuf.mtype == 2) {
            sbuf.mtype = 4;
            snprintf(sbuf.mtext, MSGSZ,
                     "Respuesta privada para cliente 2: solicitud recibida");
        }
        else {
            continue;
        }

        buf_length = strlen(sbuf.mtext) + 1;

        if (msgsnd(msqid, &sbuf, buf_length, 0) < 0) {
            perror("msgsnd");
            exit(1);
        }

        printf("Servidor envia respuesta de tipo %ld\n",
               sbuf.mtype);
    }

    exit(0);
}