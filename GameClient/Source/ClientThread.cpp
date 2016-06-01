#include "ClientThread.h"

SOCKET serverSock;
int playerID = -1;

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
	if( retval == SOCKET_ERROR ){
		err_quit( "connect()" );
		exit(0);
	}
	else {
		printf("Login \n");
		_sleep(1000);
	}

	char readString[BUFSIZE+1];

	//ZeroMemory( &g_worldData, sizeof(g_worldData) );

	/*retval = recvn( serverSock, (char*)&playerID, sizeof( playerID ), 0 );
	if( retval == SOCKET_ERROR ) {
		err_display( "recv()" );
	}
	else
	{
		printf("Player ID %d\n", playerID);
	}
*/
	while( 1 ){
		// Get Player Position Information by Server
		/*retval = recvn( serverSock, (char*)&g_worldData, sizeof( g_worldData ), 0 );
		if( retval == SOCKET_ERROR ) {
			err_display( "recv()" );
			break;
		}
		else if( retval == 0 )
			break;*/

		/*Vector2i setPos;
		setPos.x = g_worldData.playerInfo[playerID].pos.x;
		setPos.y = g_worldData.playerInfo[playerID].pos.y;
		g_Player[playerID].SetWorldposition( setPos );*/
	}
	closesocket( serverSock );
	serverSock = NULL;

	WSACleanup();
	exit(0);
	return 0;
}