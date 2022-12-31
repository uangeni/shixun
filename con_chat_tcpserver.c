#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <semaphore.h>
 
#define BUF_SIZE 100
#define MAX_CLNT 256
 
void *handle_clnt(void *arg);
void send_msg(char *msg, int len);
void error_handling(char *message);
 
int clnt_cnt=0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutex;
 
int main(int argc, char *argv[])
{
  int serv_sock;
  int clnt_sock;
  
  struct sockaddr_in serv_addr;
  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_size;
  
  pthread_t t_id;
  
  if(argc!=2)
  {
    exit(1);
  }
  
  pthread_mutex_init(&mutex, NULL);	//初始化信号量
  //TCP socket
  serv_sock=socket(PF_INET, SOCK_STREAM, 0);
  if(serv_sock == -1)
    error_handling("socket error!");
 
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;	//IPV4协议族
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));	//端口号
 
  if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
    error_handling("bind error");
 
  if(listen(serv_sock, 5) == -1)
    error_handling("listen error");
 
  while(1)
  {
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    
    pthread_mutex_lock(&mutex);
    clnt_socks[clnt_cnt++]=clnt_sock;
    pthread_mutex_unlock(&mutex);
    
    pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
    //pthread_join:调用该函数的线程将进入阻塞状态，等待结束后回收其资源
    //pthread_detach:调用该函数不会引起线程阻塞，线程结束后会自动释放所有资源
    pthread_detach(t_id);
    printf("connected client IP: %s \n", inet_ntoa(clnt_addr.sin_addr));
  }
  
  close(serv_sock);
  return 0;
}
 
void* handle_clnt(void* arg)
{
  int clnt_sock=*((int *)arg);
  int str_len=0;
  char msg[BUF_SIZE];
  
  while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0)
    send_msg(msg, str_len);
  
  pthread_mutex_lock(&mutex);
  int i;
  for(i=0;i<clnt_cnt;i++)	//删除断开连接的客户端
  {
    if(clnt_sock==clnt_socks[i])
    {
      while(i++<clnt_cnt-1)
	clnt_socks[i]=clnt_socks[i+1];
      break;
    }
  }
  clnt_cnt--;
  pthread_mutex_unlock(&mutex);
  close(clnt_sock);
  return NULL;
}
 
void send_msg(char* msg, int len)
{
  int i;
  pthread_mutex_lock(&mutex);
  for(i=0;i<clnt_cnt;i++)	//向所有连接的客户端发送消息
    write(clnt_socks[i], msg, len);
  pthread_mutex_unlock(&mutex);
}
 
void error_handling(char *message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}