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
	printf("���� ip �ּҸ� �Է��Ͻÿ�: ");
	scanf("%s", SERVERIP);

	printf("GAME ID�� �Է��Ͻÿ�: ");
	scanf("%s", gameid);

	printf("GAME PASSWORD�� �Է��Ͻÿ�: ");
	scanf("%s", gamepass);

	return 0;
}