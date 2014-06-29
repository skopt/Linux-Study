#ifndef _DATATRAN_H_
#define _DATATRAN_H_
#include <sys/socket.h>
#include <netinet/in.h>
#include <map>
#include <pthread.h>
//#include "cmdproc.h"
using namespace std;
typedef int (*OneCharParam)(char *param);

struct ClientInfo
{
    int Sock;
    sockaddr_in SockAddr;
};
class CDataTran
{
public:
CDataTran();
~CDataTran();
public:
   OneCharParam DataRecvProc; 
   map<int, sockaddr_in> ClientInfoMap;
   pthread_mutex_t ClientInfoMapLock;
private:
    int m_iLisSock;
    int m_iPort;

public:
    bool Start(int port);
    static void* RecvTheadFun(void *pArgu);
private:
    bool InitSock(int port);
    static void* LisThreadFun(void *pArgu);
	int RecvDataProc(char *buff, int len, int Confd);
    
};
#endif