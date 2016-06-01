#include "Common.h"
#include "ClientThread.h"
#include "Object\World.h"
#include "Object\Player.h"
#include "System\Display.h"

int main()
{
	
	srand((unsigned int)time(NULL));

	char SERVERIP[20];
	char gameid[255];
	char gamepass[255];
	printf("서버 ip 주소를 입력하시오: ");
	scanf("%s", SERVERIP);

	printf("GAME ID를 입력하시오: ");
	scanf("%s", gameid);

	printf("GAME PASSWORD를 입력하시오: ");
	scanf("%s", gamepass);

	return 0;
}