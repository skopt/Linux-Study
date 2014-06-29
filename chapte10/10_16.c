#include <stdio.h>
#include <signal.h>

int flag;
static void handler(int signo);

void main()
{
    sigset_t newmask, oldmask, zeromask;
    flag = 0;

    if(signal(SIGINT, handler) == SIG_ERR)
        printf("reg sig error");
    if(signal(SIGQUIT, handler) == SIG_ERR)
        printf("reg sig error");

    sigemptyset(&newmask);
    sigemptyset(&zeromask);
    sigaddset(&newmask, SIGQUIT);

    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        printf("sigprocmask error");

    while(0 == flag)
        sigsuspend(&zeromask);

    flag = 0;

    if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        printf("reset sig error");
    
}

static void handler(int signo)
{
    if(SIGINT == signo)
        printf("recive sig int\n");
    else if(SIGQUIT ==  signo)
        flag = 1;
}
