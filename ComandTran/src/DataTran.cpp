#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include "DataTran.h"
#include "log.h"
#include "cmdproc.h"

struct RecvThreadParam
{
int Confd;
CDataTran *pthis;
};

#define RECV_BUFF_LEN 1024
CDataTran::CDataTran()
{
   log_init();
   ClientInfoMap.clear();
}
CDataTran::~CDataTran()
{
    close(m_iLisSock);
}
bool CDataTran::Start(int port)
{
	m_iPort = port;
	pthread_t ThreadId;
	pthread_create(&ThreadId, NULL, LisThreadFun,(void *)this);
	if(0 != ThreadId)
    {
        dbg_log("creat thread error");
        return false;
    }
	return true;
}
bool CDataTran::InitSock(int port)
{
    sockaddr_in ServAddr;
    
    m_iLisSock = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == m_iLisSock)
        return false;
	int opt = 1;
	setsockopt(m_iLisSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt));
    //init addr
    memset(&ServAddr, 0, sizeof(sockaddr_in));
    ServAddr.sin_family = AF_INET;
    ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    ServAddr.sin_port = htons(port);
    //bind    
    if( bind(m_iLisSock, (struct sockaddr*)&ServAddr, sizeof(ServAddr)) == -1)
    {
        close(m_iLisSock);
        return false;
    }	
    //listen    
    if( listen(m_iLisSock, 10) == -1)
    {
        close(m_iLisSock);
        return false;
    }
    
    return true;
}
void* CDataTran::LisThreadFun(void *pArgu)
{
	CDataTran *pthis = (CDataTran *)pArgu;
	sockaddr_in c_add;
    int Confd = 0, sock = 0, DataLenRecv = 0;
    char buff[RECV_BUFF_LEN];
	pthread_t thId;
	RecvThreadParam RecvThParam;
	log_write("in thread", strlen("in thread"));
	//sock = pthis->m_iLisSock;
	socklen_t addrlen = sizeof(sockaddr_in);

	if(false == pthis->InitSock(pthis->m_iPort))
	{
		dbg_log("init socket failure");
		return (void *)NULL;
	}
	
    while(1)
	{		
		printf("begin to accept, sock:%d\n", sock);
	    if( (Confd = accept(pthis->m_iLisSock, (sockaddr*)&c_add, &addrlen)) == -1)
		{	        
			dbg_log("accept error");
			sleep(3);
	        continue;
	    }
		printf("accepted con:%d\n", Confd);
		dbg_log("accept a connection");
		RecvThParam.Confd = Confd;
		RecvThParam.pthis = pthis;
		pthread_create(&thId, NULL, CDataTran::RecvTheadFun, (void *)&RecvThParam);
		if(0 != thId)
		{
		    dbg_log("creat recvthread error");
		}
		pthread_mutex_lock(&pthis->ClientInfoMapLock);
		pthis->ClientInfoMap[Confd] = c_add;
		pthread_mutex_unlock(&pthis->ClientInfoMapLock);
    }
}

void* CDataTran::RecvTheadFun(void *pArgu)
{
	RecvThreadParam *pParam = (RecvThreadParam *)pArgu;
	int Confd = pParam->Confd;
	CDataTran *pthis = pParam->pthis;
	int DataLenRecv = 0;
	char buff[RECV_BUFF_LEN];
	printf("begin to recv, Con:%d\n", Confd);
	while(1)
	{
		memset(buff, 0, RECV_BUFF_LEN);
		DataLenRecv = recv(Confd, buff, RECV_BUFF_LEN, 0);
		if(DataLenRecv <= 0)
		{
			printf("recv len < 0\n");
			close(Confd);
			break;
		}
		//process the data receieve
		pthis->RecvDataProc(buff, DataLenRecv, Confd);	    	
	}

	//delete the close socket
	printf("the size before del is %d\n", pthis->ClientInfoMap.size());
	pthread_mutex_lock(&pthis->ClientInfoMapLock);
	pthis->ClientInfoMap.erase(Confd);
	pthread_mutex_unlock(&pthis->ClientInfoMapLock);
	printf("the size after del is %d\n", pthis->ClientInfoMap.size());
}

int CDataTran::RecvDataProc(char *buff, int len, int Confd)
{
    buff[len] = '\0';
    printf("recv msg from client: %s\n", buff);
    Cmdproc::RecvDataProc(buff, len, Confd);
	/*
	map<int,sockaddr_in>::iterator it;
	for(it = pthis->ClientInfoMap.begin();it != pthis->ClientInfoMap.end(); it++)
	{
	   send(it->first, buff, strlen(buff), 0);
	}
	*/
	return 0;
}