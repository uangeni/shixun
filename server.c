#include "comm.h"

int main()
{
    int msgid = createMsgQueue();
    printf("msgid: %d\n ",msgid);
    char buf[SIZE];
    while(1){
        recvMsg(msgid ,CLIENT_TYPE,buf);
        printf("client# %s\n",buf);
        printf("Please Enter$ ");
        fflush(stdout);
        ssize_t s = read(0,buf,sizeof(buf)-1);
        if(s > 0){
            buf[s -1] ='\0';
            sendMsg(msgid,SERVER_TYPE,buf);
        }
    }
    DestroyMsgQueue(msgid);
    return 0;
}