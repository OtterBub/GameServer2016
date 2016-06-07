#include "Common.h"
#include "System\MGR\ThreadMgr.h"
#include "System\Connect.h"
#include "Object\Player.h"
#include "System\mdump.h"

int main(int argc, char** argv){	

	CMiniDump::Begin();

	ThreadMgr lWorkerThreadMgr;

	Connect::ConnectInitialize();
	lWorkerThreadMgr.PushThread(new std::thread{ Connect::AcceptThread });

	for( auto i = 0; i < NUM_THREADS; ++i )
		lWorkerThreadMgr.PushThread(new std::thread{ Connect::WorkerThread });

	lWorkerThreadMgr.Join();
	lWorkerThreadMgr.Clear();

	CMiniDump::End();
	return 0;
}