#ifndef __CONNECTSYS_H__
#define __CONNECTSYS_H__
#include "../Common.h"


struct OverlapEx
{
	WSAOVERLAPPED OriginalOverlap;
	int operation;
	WSABUF wsabuf;
	unsigned char iocp_buff[MAX_BUFF_SIZE];
};

class Connect
{
public:
	static void ConnectInitialize();
	static void AcceptThread();
	static void WorkerThread();
	
	
	static void SendPacket(void *packet, unsigned int key);
	static void SendBroadCasting(void* packet, unsigned int except = MAX_USER);
	static void ProcessPacket(unsigned char* packet, unsigned int key);
	
public:

private:
	OverlapEx mOverlap;
};


#endif

