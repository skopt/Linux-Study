#include <stdio.h>
#include <sys/wait.h>
//1-5 process
static void sig_int(int signo);
void example1_5();

void main()
{
  //-----------------------usid&gid----------
  int uid = 0, gid = 0;
  uid = getuid();
  gid = getgid();
  printf("userID=%d, groupID=%d\n", uid, gid);

  example1_5();
}

//1-5 process
void example1_5()
{
    char buf[1024];
    pid_t pid = 0;
    int status;

    if(signal(SIGINT, sig_int) ==SIG_ERR)
        printf("signal error\n");
    while(fgets(buf, 1024, stdin) != NULL)
    {
        pid = fork();
        if(pid < 0)
        {
            printf("fork error\n");
        }
        else if(0 == pid)
        {
            printf("child excute\n");
            exit(127);
            printf("child not run\n");
        }
        pid = waitpid(pid, &status, 0);
        if(pid < 0)
        {
            printf("wait pid error\n");
        }
        printf("over pid=%d, status=%d\n", pid, status);
    }
    
}

void sig_int(int signo)
{
    printf("signal=%d\n",signo);
}
