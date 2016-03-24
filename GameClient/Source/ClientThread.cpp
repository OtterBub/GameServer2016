#include "ClientThread.h"

SOCKET serverSock;

DWORD WINAPI ClientMain(LPVOID arg)
{	
	int retval;

	WSADATA wsa;
	if( WSAStartup( MAKEWORD( 2, 2 ), &wsa ) != 0 )
		return 1;

	serverSock = socket( AF_INET, SOCK_STREAM, 0 );
	if( serverSock == INVALID_SOCKET )
		err_quit( "socket()" );

	char SERVERIP[20];
	strcpy(SERVERIP, (char*)arg);

	SOCKADDR_IN serveraddr;
	ZeroMemory( &serveraddr, sizeof( serveraddr ) );
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr( SERVERIP );
	serveraddr.sin_port = htons( SERVERPORT );
	retval = connect( serverSock, ( SOCKADDR * )&serveraddr, sizeof( serveraddr ) );
	if( retval == SOCKET_ERROR )
		err_quit( "connect()" );
	else {
		printf("Login \n");
		_sleep(1000);
	}

	char readString[BUFSIZE+1];

	playerPos pos;
	ZeroMemory( &pos, sizeof(pos) );

	while( 1 ){
		retval = recvn( serverSock, (char*)&pos, sizeof( pos ), 0 );
		if( retval == SOCKET_ERROR ) {
			err_display( "recv()" );
			break;
		}
		else if( retval == 0 )
			break;

		Vector2i setPos;
		setPos.x = pos.x;
		setPos.y = pos.y;
		g_Player.SetWorldposition( setPos );
	}
	closesocket( serverSock );
	serverSock = NULL;

	WSACleanup();

	return 0;
}