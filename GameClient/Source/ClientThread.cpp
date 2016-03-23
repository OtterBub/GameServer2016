#include "ClientThread.h"

DWORD WINAPI ClientMain(LPVOID arg)
{	
	int retval;

	WSADATA wsa;
	if( WSAStartup( MAKEWORD( 2, 2 ), &wsa ) != 0 )
		return 1;

	sock = socket( AF_INET, SOCK_STREAM, 0 );
	if( sock == INVALID_SOCKET )
		err_quit( "socket()" );

	char SERVERIP[20];
	strcpy(SERVERIP, (char*)arg);
	/*printf( "서버 ip 주소를 입력하시오: " );
	scanf( "%s", SERVERIP );
	fflush( stdin );*/
	/*printf("\n");
	printf(SERVERIP);*/

	SOCKADDR_IN serveraddr;
	ZeroMemory( &serveraddr, sizeof( serveraddr ) );
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr( SERVERIP );
	serveraddr.sin_port = htons( SERVERPORT );
	retval = connect( sock, ( SOCKADDR * )&serveraddr, sizeof( serveraddr ) );
	if( retval == SOCKET_ERROR )
		err_quit( "connect()" );
	else {
		printf("Login \n");
		_sleep(1000);
	}

	char readString[BUFSIZE+1];
	int len;
	int playerID = 0;

	retval = recv(sock, (char*)&playerID, sizeof(playerID), 0);
	if(retval == SOCKET_ERROR){
		err_display("recv()");
	}

	while( 1 ){
		/*retval = recvn(sock, (char*)&WorldInfo, sizeof(WorldData), 0);
		if(retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		} else if(retval == 0)
			break;*/
	}
	closesocket( sock );

	WSACleanup();

	return 0;
}