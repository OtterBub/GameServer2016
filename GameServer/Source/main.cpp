#include "Thread\Thread.h"
#include "System\WorkerThreadMgr.h"
#include "System\ConnectSystem.h"

void print(int num)
{
	int myNum = num;

	while (1)
	{
		std::wcout << L"HelloWorld >> " << myNum << L" << \n";
		Sleep(1);
	}
	
}


int main(int argc, char** argv){
	WorkerThreadMgr lWorkerThreadMgr;

	lWorkerThreadMgr.PushThread(new std::thread{ ConnectSystem::WorkerThread });
	lWorkerThreadMgr.PushThread(new std::thread{ ConnectSystem::WorkerThread });
	lWorkerThreadMgr.PushThread(new std::thread{ ConnectSystem::WorkerThread });
	lWorkerThreadMgr.PushThread(new std::thread{ ConnectSystem::WorkerThread });
	lWorkerThreadMgr.PushThread(new std::thread{ ConnectSystem::WorkerThread });


	lWorkerThreadMgr.Join();
	lWorkerThreadMgr.Clear();

	return 0;
}