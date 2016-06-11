#include "Common.h"
#include "System\MGR\ObjectMgrList.h"
#include "System\MGR\ThreadMgr.h"
#include "System\Connect.h"
#include "Object\Player.h"
#include "System\mdump.h"

// keyinput
#include <conio.h>
void keyboardTest()
{
	char key = 0;
	while (false == GLOBAL.mShutdown)
	{
		key = _getch();
		OverlapEx *over = new OverlapEx;
		over->operation = OP_TEST;
		PostQueuedCompletionStatus(GLOBAL.mhIocp, 1, key, &over->OriginalOverlap);
	}
}

int main(int argc, char** argv){	

	CMiniDump::Begin();

	ThreadMgr lWorkerThreadMgr;

	//init test
	{
		for (unsigned int i = 0; i < 10000; ++i)
		{
			if (false == NPCMGR.ExistClient(i))
			{
				NPC(i).is_active = true;
			}
		}
	}

	Connect::ConnectInitialize();
	lWorkerThreadMgr.PushThread(new std::thread{ Connect::AcceptThread });

	for( auto i = 0; i < NUM_THREADS; ++i )
		lWorkerThreadMgr.PushThread(new std::thread{ Connect::WorkerThread });

	lWorkerThreadMgr.PushThread(new std::thread{ keyboardTest });

	lWorkerThreadMgr.Join();
	lWorkerThreadMgr.Clear();

	CMiniDump::End();
	return 0;
}