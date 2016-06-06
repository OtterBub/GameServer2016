#include "Common.h"
#include "ClientThread.h"
#include "Object\World.h"
#include "Object\Player.h"
#include "System\Display.h"
#include "System\ThreadMgr.h"
#include "System\ClientConnect.h"
#include "System\mdump.h"

int main()
{
	CMiniDump::Begin();

	ThreadMgr lThreadMgr;

	srand((unsigned int)time(NULL));

	std::string SERVERIP;
	std::string gameid;
	std::string gamepass;
	std::cout << "서버 ip 주소를 입력하시오: ";
	std::cin >> SERVERIP;

	std::cout << "GAME ID를 입력하시오: ";
	std::cin >> gameid;

	std::cout << "GAME PASSWORD를 입력하시오: ";
	std::cin >> gamepass;

	CONNECT.Initialize();
	CONNECT.Connect( SERVERIP.c_str() );

	lThreadMgr.PushThread(new std::thread{ Display::Initialize });
	lThreadMgr.Join();

	CMiniDump::End();
	return 0;
}