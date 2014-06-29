#include <stdio.h>
#include <signal.h>
#include <pwd.h>
#include <sys/wait.h>
void test_01();
static void sig_usr(int signo);

void test_02();
static void myAlarm(int signo);
static void mySegv(int signo);

void test_03();
static void myChld(int signo);


void main()
{
    //test_01();
    //test_02();
    test_03();
    
}
void test_01()
{
    if(signal(SIGUSR1, sig_usr) == SIG_ERR)
    {
        printf("error");
    }

    if(signal(SIGUSR2, sig_usr) == SIG_ERR)
    {
        printf("Error");
    }

    for(;;)
        pause();
}
static void sig_usr(int signo)
{
   if(SIGUSR1 == signo)
    printf("User 1\n");
   else if(SIGUSR2 == signo)
    printf("User 2\n");
   else
    printf("Other\n");
}

/////////////////////////////
void test_02()
{
    struct passwd *ptr;

    signal(SIGALRM, myAlarm);
    signal(SIGSEGV, mySegv);
    alarm(1);

    for(; ;)
    {
        if((ptr = getpwnam("skopt")) == NULL)
            printf("get pw name error");
        if(strcmp(ptr->pw_name, "sar") != 0)
            printf("Error, pw_name=%s\n", ptr->pw_name);
        else
            printf("Right, pw_name=%s\n", ptr->pw_name);
    }    
}

static void myAlarm(int signo)
{
    struct passwd * ptr;

    printf("in myAlarm\n");

    if((ptr = getpwnam("skopt")) == NULL)
        printf("get pw nmae error in handler\n");

    alarm(1);
}

static void mySegv(int signo)
{
    printf("into mySegv\n");
}

/////////////////////

void test_03()
{
    pid_t pid = 0;

    if(signal(SIGCHLD, myChld) == SIG_ERR)
        printf("signal error\n");
    if((pid = fork()) < 0)
        printf("fork error\n");
    else if(pid == 0)
    {
        sleep(3);
        _exit(0);
    }

    pause();    
}

static void myChld(int signo)
{
    int status, pid;

    printf("CHLD received\n");
    
    if(signal(SIGCHLD, myChld) == SIG_ERR)
        printf("signal error in myChld\n");

    if((pid = wait(&status)) < 0)
        printf("wait error\n");

    printf("pid=%d, status=%d\n", pid, status);
}

