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
    key_t key = 1234;
    message_buf sbuf, rbuf;
    size_t buf_length;

    // Acceder a la cola existente
    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    // Enviar solicitud al servidor
    sbuf.mtype = 2;
    strcpy(sbuf.mtext, "Solicitud del cliente 2");
    buf_length = strlen(sbuf.mtext) + 1;

    if (msgsnd(msqid, &sbuf, buf_length, 0) < 0) {
        perror("msgsnd");
        exit(1);
    }

    printf("Cliente 2 envia: %s\n", sbuf.mtext);

    // Recibir exclusivamente la respuesta del cliente 2
    if (msgrcv(msqid, &rbuf, MSGSZ, 4, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }

    printf("Cliente 2 recibe: %s\n", rbuf.mtext);

    exit(0);
}