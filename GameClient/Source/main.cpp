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
	std::cout << "���� ip �ּҸ� �Է��Ͻÿ�: ";
	std::cin >> SERVERIP;

	std::cout << "GAME ID�� �Է��Ͻÿ�: ";
	std::cin >> gameid;

	std::cout << "GAME PASSWORD�� �Է��Ͻÿ�: ";
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