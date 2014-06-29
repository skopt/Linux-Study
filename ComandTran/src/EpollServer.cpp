#include "EpollServer.h"
#include <sys/types.h>
#include <sys/resource.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

CEpollServer::CEpollServer(int port)
{
   m_iPort = port;
}
bool CEpollServer::InitEvn()
{
	struct rlimit rt;
    
    rt.rlim_max = rt.rlim_cur = EPOLL_EVENT_MAX;
    if (setrlimit(RLIMIT_NOFILE, &rt) == -1) 
    {
        perror("setrlimit error");
        return false;
    }
	return true;
}

bool CEpollServer::InitScoket()
{
	sockaddr_in v_SerAddr;
	memset(&v_SerAddr, 0, sizeof(sockaddr_in));
	v_SerAddr.sin_family = AF_INET;
	v_SerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	v_SerAddr.sin_port = htons(m_iPort);
	do
	{
		//init socket
		if((m_iListenSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			break;
		}
		//set socket
		int opt = 1;
		setsockopt(m_iListenSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt));
		if(fcntl(m_iListenSock, F_SETFL, fcntl(m_iListenSock, F_GETFD, 0)|O_NONBLOCK) == -1)
		{
			break;
		}
		//bind    
		if( bind(m_iListenSock, (struct sockaddr*)&v_SerAddr, sizeof(v_SerAddr)) == -1)
		{
		    break;
		}
		//listen 
		if(listen(m_iListenSock, 1024) == -1)
		{
			break;
		}
		if(false == AddLisSockEvent())
		{
			break;
		}
		return true;
	}while(0);
	close(m_iListenSock);
	return false;
}
bool CEpollServer::AddLisSockEvent()
{
	m_iEpollfd = epoll_create(EPOLL_EVENT_MAX);
	m_ListenEvent.events = EPOLLIN|EPOLLET;
	m_ListenEvent.data.fd = m_iListenSock;
	if(epoll_ctl(m_iEpollfd, EPOLL_CTL_ADD, m_iListenSock, &m_ListenEvent) < 0)
	{
		return false;
	}
	return true;
}

bool CEpollServer::Start()
{	
    if(!InitEvn())
    {
		printf("init env failed\n");
		return false;
    }    
	
    pthread_t v_ThreadID;	
	int ret = pthread_create(&v_ThreadID, NULL, _EventRecvFun, (void *)this);
	if(0 != ret)
	{
		printf("thread creat failed\n");
		return false;
	}
	return true;
}
void* CEpollServer::_EventRecvFun(void *pArgu)
{
	CEpollServer *pthis = (CEpollServer *)pArgu;
	int v_NumRecvFD = 0, v_NumCurFD = 1;
	if(!pthis->InitScoket())
	{
		printf("init sock failed\n");
		return (void *)0;
	}
	printf("into eventRecvFun, init socket succeed\n");
	//recv
	while(true)
	{
		v_NumRecvFD = epoll_wait(pthis->m_iEpollfd, pthis->m_Events,v_NumCurFD, -1);
		if(v_NumCurFD < 0)
		{
			printf("epoll wait error\n");
			continue;
		}
		printf("v_NumRecvFD=%d, v_NumCurFD=%d\n", v_NumRecvFD, v_NumCurFD);
		pthis->ProcessRecvEnts(v_NumRecvFD, v_NumCurFD);
	}
	
	
}
bool CEpollServer::ProcessRecvEnts(int NumRecvFD, int &v_NumCurFD)
{
	int connfd = 0;
	sockaddr_in v_ClntAddr;	
	socklen_t addrlen = sizeof(sockaddr_in);
	epoll_event ev;
	for(int i = 0; i < NumRecvFD; i++)
	{
		if (m_Events[i].data.fd == m_iListenSock) 
        {
            connfd = accept(m_iListenSock, (struct sockaddr *)&v_ClntAddr,&addrlen);
            if (connfd < 0)
            {
				printf("accept error\n");
				continue;
            }
			printf("recv a new client\n");
			fcntl(connfd, F_SETFL, fcntl(connfd, F_GETFD, 0)|O_NONBLOCK);
			ev.events = EPOLLIN | EPOLLET;
            ev.data.fd = connfd;
			if(epoll_ctl(m_iEpollfd, EPOLL_CTL_ADD, connfd, &ev) < 0)
			{
				printf("Add connfd error\n");
				close(connfd);
				continue;
			}
			v_NumCurFD++;
			continue;
		}
        ProcessRecvData(m_Events[i]);
		
	}

    return true;
}
bool CEpollServer::ProcessRecvData(epoll_event event)
{
    int nread, MAXLINE=1024;
    char buf[MAXLINE];
    nread = read(event.data.fd, buf, MAXLINE);
    if (nread == 0) {
        printf("client close the connection\n");
        close(event.data.fd);
        return -1;
    } 
    if (nread < 0) {
        perror("read error");
        close(event.data.fd);
        return -1;
    }    
    write(event.data.fd, buf, nread);
    return true;
}