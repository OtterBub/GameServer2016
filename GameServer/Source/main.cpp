#include "Thread\Thread.h"
#include "System\ThreadMgr.h"
#include "System\ConnectSystem.h"

int main(int argc, char** argv){
	ThreadMgr lWorkerThreadMgr;

	ConnectSystem::ConnectInitialize();
	lWorkerThreadMgr.PushThread(new std::thread{ ConnectSystem::AcceptThread });

	for( auto i = 0; i < NUM_THREADS; ++i )
		lWorkerThreadMgr.PushThread(new std::thread{ ConnectSystem::WorkerThread });

	lWorkerThreadMgr.Join();
	lWorkerThreadMgr.Clear();

	return 0;
}