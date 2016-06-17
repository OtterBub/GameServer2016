#include "Common.h"
#include "System\ThreadMgr.h"
#include "System\ClientConnect.h"
#include "System\mdump.h"
#include "ClientFrame\System\DrawSystem.h"

#define LOCALTEST 0

int main()
{
	CMiniDump::Begin();

	ThreadMgr lThreadMgr;

	srand((unsigned int)time(NULL));

	std::string SERVERIP;
	
	std::string gamepass;

	CONNECT.Initialize();
#if !LOCALTEST
	std::cout << "서버 ip 주소를 입력하시오: ";
	//std::cin >> SERVERIP;

	CONNECT.Connect("127.0.0.1");
	//CONNECT.Connect(SERVERIP.c_str());

#else
	CONNECT.Connect("127.0.0.1");
	//CONNECT.Connect("222.120.172.245");
#endif
	lThreadMgr.PushThread(new std::thread{ ClientConnect::ReadPacket });

	DrawSystem sys;
	sys.Init();
	sys.Run();

	lThreadMgr.Join();

	CMiniDump::End();

	return 0;
}