#include <stdio.h>
#include <string.h>
#include <cmdproc.h>
#include <DataTran.h>
#include "EpollServer.h"

int CommandResultPro(char *ret);

int main()
{
    char buff[1024];
	CEpollServer tmp(6005);
	tmp.Start();
	/*
	CDataTran tmp;
	tmp.DataRecvProc = CommandResultPro;
	tmp.Start(6005);
	*/
    while(1)
    {
        scanf("%s", buff);
        CommandPro(buff,CommandResultPro); 
    }
    char *pComand = "ls";
    CommandPro(pComand,CommandResultPro);
	
	return 1;
}

int CommandResultPro(char *ret)
{
    printf("%s\n", ret);
}
