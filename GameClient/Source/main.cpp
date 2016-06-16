#include "Common.h"
#include "System\ThreadMgr.h"
#include "System\ClientConnect.h"
#include "System\mdump.h"
#include "ClientFrame\System\DrawSystem.h"

#define LOCALTEST 1

int main()
{
	CMiniDump::Begin();

	ThreadMgr lThreadMgr;

	srand((unsigned int)time(NULL));

	std::string SERVERIP;
	std::string gameid;
	std::string gamepass;

	CONNECT.Initialize();
#if !LOCALTEST
	std::cout << "서버 ip 주소를 입력하시오: ";
	std::cin >> SERVERIP;

	std::cout << "GAME ID를 입력하시오: ";
	std::cin >> gameid;

	std::cout << "GAME PASSWORD를 입력하시오: ";
	std::cin >> gamepass;
	CONNECT.Connect(SERVERIP.c_str());

#else
	CONNECT.Connect("127.0.0.1");
#endif
	lThreadMgr.PushThread(new std::thread{ ClientConnect::ReadPacket });

	DrawSystem sys;
	sys.Init();
	sys.Run();

	lThreadMgr.Join();

	CMiniDump::End();
	return 0;
}