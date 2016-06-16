#include "Common.h"
#include "System\MGR\ObjectMgrList.h"
#include "System\MGR\ThreadMgr.h"
#include "System\Connect.h"
#include "Object\Player.h"
#include "System\mdump.h"
#include "System\MGR\EventMgr.h"
#include "System\MGR\DataBaseMgr.h"

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
		for (unsigned int i = 0; i < 1000; ++i)
		{
			if (false == NPCMGR.ExistClient(i))
			{
				NPC(i).is_active = false;
				NPC(i).info.mPos = Vector3f(rand() % 100, rand() % 100, 0);
			}
		}		
	}

	Connect::ConnectInitialize();
	lWorkerThreadMgr.PushThread(new std::thread{ Connect::AcceptThread });

	for( auto i = 0; i < NUM_THREADS; ++i )
		lWorkerThreadMgr.PushThread(new std::thread{ Connect::WorkerThread });

	lWorkerThreadMgr.PushThread(new std::thread{ TimerEventMgr::TimerThread });
	lWorkerThreadMgr.PushThread(new std::thread{ keyboardTest });
	lWorkerThreadMgr.PushThread(new std::thread{ DataBaseMgr::Test });

	lWorkerThreadMgr.Join();
	lWorkerThreadMgr.Clear();

	CMiniDump::End();
	return 0;
}