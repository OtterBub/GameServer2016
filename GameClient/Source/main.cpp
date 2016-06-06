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
	std::cout << "���� ip �ּҸ� �Է��Ͻÿ�: ";
	std::cin >> SERVERIP;

	std::cout << "GAME ID�� �Է��Ͻÿ�: ";
	std::cin >> gameid;

	std::cout << "GAME PASSWORD�� �Է��Ͻÿ�: ";
	std::cin >> gamepass;

	CONNECT.Initialize();
	CONNECT.Connect( SERVERIP.c_str() );

	lThreadMgr.PushThread(new std::thread{ Display::Initialize });
	lThreadMgr.Join();

	CMiniDump::End();
	return 0;
}