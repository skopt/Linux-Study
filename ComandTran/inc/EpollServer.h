#ifndef EPOLLSERVER_H
#define EPOLLSERVER_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/epoll.h>

#define EPOLL_EVENT_MAX 1024

class CEpollServer
{
public:
	

private:
	int m_iListenSock;
	int m_iPort;
	int m_iEpollfd;
	epoll_event m_ListenEvent;
	epoll_event m_Events[EPOLL_EVENT_MAX];

public:
	CEpollServer(int port);
	bool Start();

private:
	bool InitEvn();
	bool InitScoket();
	bool AddLisSockEvent();
	static void* _EventRecvFun(void *pArgu);
	bool ProcessRecvEnts(int NumRecvFD, int &v_NumCurFD);
	bool ProcessRecvData(epoll_event event);
	

};
#endif
