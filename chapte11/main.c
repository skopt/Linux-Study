#include <stdio.h>
#include <pthread.h>
void* thFun(void *arg);

void main()
{
    pthread_t pthid;
    int err;

    err = pthread_create(&pthid, NULL, thFun, NULL);
    if(err != 0)
        printf("Creat thread failure\n");
    printf("In Main, Thread ID:%u\n", pthid);
    sleep(2);
    
}
void* thFun(void *arg)
{
    pthread_t pthid;

    pthid = pthread_self();
    printf("Thread ID:%u\n", pthid);
}
