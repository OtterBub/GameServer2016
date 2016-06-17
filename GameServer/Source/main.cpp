#include "Common.h"
#include "System\MGR\ObjectMgrList.h"
#include "System\MGR\ThreadMgr.h"
#include "System\Connect.h"
#include "Object\Player.h"
#include "System\mdump.h"
#include "System\MGR\EventMgr.h"
#include "System\MGR\DataBaseMgr.h"
#include "Global.h"
#include <string>

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
		/*std::string nickQuery = "SELECT * FROM dbo.user_table WHERE Nick = '";
		std::string nick;
		std::cin >> nick;
		DBMGR.SearchNick(nick, 1000000);*/
	}
}

int main(int argc, char** argv){	

	CMiniDump::Begin();
	DBMGR.Init();
	ThreadMgr lWorkerThreadMgr;
	
	//init test
	{
		for (unsigned int i = 0; i < NPCMAX; ++i)
		{
			if (false == NPCMGR.ExistClient(i))
			{
				NPC(i).is_active = false;
				NPC(i).info.mPos = Vector3f(rand() % 100, rand() % 100, 0);
				NPC(i).info.hp = 1 + rand() % 10;

				std::wstring wstr = L"[Mob]";
				int nameCnt = 1 + (rand() % 4);
				for (int i = 0; i < nameCnt; ++i)
				{
					if( i == 0)
						wstr += (L'A') + rand() % (L'Z' + 1);
					else
						wstr += (L'a') + rand() % (L'z' + 1);

				}
				wstr.copy(NPC(i).info.char_id, wstr.size());
			}
		}		
	}

	Connect::ConnectInitialize();
	lWorkerThreadMgr.PushThread(new std::thread{ Connect::AcceptThread });

	for( auto i = 0; i < NUM_THREADS; ++i )
		lWorkerThreadMgr.PushThread(new std::thread{ Connect::WorkerThread });

	lWorkerThreadMgr.PushThread(new std::thread{ TimerEventMgr::TimerThread });
	lWorkerThreadMgr.PushThread(new std::thread{ keyboardTest });

	lWorkerThreadMgr.Join();
	lWorkerThreadMgr.Clear();

	CMiniDump::End();
	return 0;
}