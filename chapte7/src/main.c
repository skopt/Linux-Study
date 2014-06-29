#include <setjmp.h>
#include <stdio.h>
#include <sys/resource.h>
void jmp1();
void jmp2();


jmp_buf jmpbuffers;

int main()
{
    int ret = 0;
    struct rlimit limit;
    getrlimit(RLIMIT_DATA, &limit);
    printf("cur:%1011d, max:%101d\n", limit.rlim_cur, limit.rlim_max);

    ret = setjmp(jmpbuffers);
    if(ret != 0)
        printf("jup return:%d\n", ret);

    if(0 == ret)
        jmp1();
    else if(1 == ret)
        jmp2();

    printf("exit\n");
}

void jmp1()
{
    printf("into jmp1\n");
    longjmp(jmpbuffers, 1);
}

void jmp2()
{
    printf("into jmp2\n");
    longjmp(jmpbuffers, 2);
}
