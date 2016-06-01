#include "Thread\Thread.h"
#include "System\ThreadMgr.h"
#include "System\Connect.h"
#include "Object\Player.h"

int main(int argc, char** argv){	

	ThreadMgr lWorkerThreadMgr;

	Connect::ConnectInitialize();
	lWorkerThreadMgr.PushThread(new std::thread{ Connect::AcceptThread });

	for( auto i = 0; i < NUM_THREADS; ++i )
		lWorkerThreadMgr.PushThread(new std::thread{ Connect::WorkerThread });

	lWorkerThreadMgr.Join();
	lWorkerThreadMgr.Clear();

	return 0;
}