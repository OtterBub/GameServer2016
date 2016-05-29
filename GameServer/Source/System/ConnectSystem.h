#ifndef __CONNECTSYS_H__
#include "../Thread/Thread.h"
#define __CONNECTSYS_H__

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
	static void WorkerThread();

	void TestFunc()
	{
		std::cout << "hello " << mOverlap.operation << std::endl;
	}

private:
	OverlapEx mOverlap;
};


#endif

