#ifndef __CONNECTSYS_H__
#define __CONNECTSYS_H__
#include "../Thread/Thread.h"


struct OverlapEx
{
	WSAOVERLAPPED OriginalOverlap;
	int operation;
	WSABUF wsabuf;
	unsigned char iocp_buff[MAX_BUFF_SIZE];
};

class ConnectSystem
{
public:
	static void ConnectInitialize();
	static void AcceptThread();
	static void WorkerThread();

	void TestFunc()
	{
		std::cout << "hello " << mOverlap.operation << std::endl;
	}

public:

private:
	OverlapEx mOverlap;
};


#endif

