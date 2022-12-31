#ifndef _COMM_H_
#define _COMM_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define PATHNAME "."
#define PROJ_ID  0x6666
#define SIZE 128
#define SERVER_TYPE 1
#define CLIENT_TYPE 2

struct msgbuf{
    long mtype;
    char mtext[SIZE];
};
int createMsgQueue();
int getMsgQueue();
int sendMsg(int msgid,long type,const char *_info);
int recvMsg(int msgid ,long type,char out[]);
int DestroyMsgQueue(int msgid);
#endif  