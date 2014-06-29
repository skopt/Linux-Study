#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
//#include <time.h>

char * init_env[] = {"USER=unknow", "PATH=/home/mysamba/project", NULL};
void test8_5();

void test8_8();
void test8_18();

void main()
{
    //void test8_5();
    //test8_8();
    test8_18();

    /*
    char *name = getlogin();
    printf("login Name:%s\n", name);
    */
    exit(0);    
}

void test8_5()
{
    
    pid_t pid, pid1, pid2, pid3;
    int status;

    pid1 = fork();
    if(0 == pid1)
    {
        printf("pid 1\n");
        sleep(10);
        exit(0);
    }
    printf("pid1=%d\n", pid1);
    pid2 = fork();
    if(0 == pid2)
    {
        sleep(10);
        exit(0);
    }
    printf("pid1=%d\n", pid2);
    
    pid3 = fork();
    if(0 == pid3)
    {
        sleep(10);
        exit(0);
    }
    printf("pid1=%d\n", pid3);

    while((pid = wait(&status)) != -1)
    {
        printf("pid=%d, end\n", pid);
    }
}

void test8_8()
{
    pid_t pid;
    int status;
    //printf("into test8_8");
    if((pid = fork()) == 0)
    {
        if(execlp("echoall", "arg0", "arg1", (char *)0) == -1)
        {
            printf("execlp error\n");
        }
        sleep(5);
    }

    char *argv[]={"arg0", "arg1", "arg2"};
    if((pid = fork()) == 0)
    {
        if(execve("/home/mysamba/project/chapte8/echoall", argv, init_env) == -1)
        {
            printf("execve error\n");
        }
        sleep(5);
    }

    while((pid = wait(&status)) != -1)
    {
        printf("pid=%d, end\n", pid);
    }
}

void test8_18()
{
    struct tms tmstart, tmend;
    clock_t start, end;
    long clktck = sysconf(_SC_CLK_TCK);

    if((start = times(&tmstart)) == -1)
    {
        printf("error to get tmstart");
    }

    if(system("date") < 0)
    {
        printf("system error\n");
    }

    if((end = times(&tmend)) == -1)
    {
        printf("error to get tmend\n");
    }

    printf("real: %7.2f\n", (end - start)/(double)clktck);
    printf("user: %7.2f\n", 
        (tmend.tms_utime - tmstart.tms_utime)/(double)clktck);
}
