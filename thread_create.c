#include <stdio.h>
#include <pthread.h>

void *myThread1(void)
{
    int i;
    for (i=0; i<100; i++)
    {
        printf("黄\n");
        sleep(1);//Let this thread to sleep 1 second,and then continue to run
    }
}

void *myThread2(void)
{
    int i;
    for (i=0; i<100; i++)
    {
        printf("文迪\n");
        sleep(1);
    }
}

int main()
{
    int ret = 0;
    pthread_t id1, id2;
  
    ret = pthread_create(&id2, NULL, (void*)myThread1, NULL);
    if (ret != 0)
    {
        printf("Create pthread error!\n");
        return -1;
    }
  
    ret = pthread_create(&id2, NULL, (void*)myThread2, NULL);
    if (ret != 0)
    {
        printf("Create pthread error!\n");
        return -1;
    }
  
    //wait for thread finishing
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
  
    return 0;
}
