#include "Thread.h"

struct clientListInfo
{
	SOCKET* cliensSock;
	bool use;
};
clientListInfo clientAddList[50] = {0,};

playerPos g_playerPos;
SOCKET g_playerSocket;
WorldData g_worldData;

// 소켓 함수 오류 출력 후 종료
void err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	
	LocalFree(lpMsgBuf);
}

int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = -1;

	while(( left > 0) || ( -1 == left )) {
		if( -1 == left )
			received = recv(s, ptr, sizeof(ptr), flags);
		else
			received = recv(s, ptr, left, flags);

		if(received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if(received == 0)
			break;

		if (-1 == left)
			left = ptr[0];
		printf("%d\n", left);
		left -= received;
		ptr += received;
	}
	return 0;
}

DWORD WINAPI CreatePlayerSocket(LPVOID arg)
{
	int retval;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) err_quit("socket()");

	// -------------------- Non - Blocking --------------------------
	u_long on = 1;
	retval = ioctlsocket(listen_sock, FIONBIO, &on);
	if(listen_sock == INVALID_SOCKET) err_quit("ioctlsocket()");

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if(retval == SOCKET_ERROR) err_quit("listen()");


	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;
	DWORD ThreadId;
	while(1) {
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if(client_sock == SOCKET_ERROR) {
			if(WSAGetLastError() != WSAEWOULDBLOCK) {
				err_display("recv()");
				break;
			}
			continue;
		}
		g_playerSocket = client_sock;
		printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\r\n", 
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		hThread = CreateThread(NULL, 0, PlayerSoketThread,
			(LPVOID)client_sock, 0, &ThreadId);
		if(hThread == NULL)
			printf("[오류] 스레드 생성 실패!\r\n");
		else
			CloseHandle(hThread);
	}
	closesocket(listen_sock);
	return 0;
}

DWORD WINAPI PlayerSoketThread(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int playerIndex;
	int retval;

	for( int i = 0; i < 50; i++ )
	{
		if( clientAddList[i].use == false )
		{

			retval = send( client_sock, (char*)&i, sizeof( i ), 0 );
			if( retval == SOCKET_ERROR ) {
				if( WSAGetLastError() != WSAEWOULDBLOCK ) {
					err_display( "send()" );
					continue;
				}
			}

			clientAddList[i].use = true;
			clientAddList[i].cliensSock = &client_sock;
			playerIndex = i;
			break;
		}
	}
	g_worldData.playerInfo[playerIndex].Id = playerIndex;
	g_worldData.playerInfo[playerIndex].login = true;

	BOOL bPotVal = TRUE;
	int bOptLen = sizeof(BOOL);

	setsockopt(client_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&bPotVal, bOptLen);

	char buf[BUFSIZE + 1];
	SOCKADDR_IN clientaddr;
	int addrlen;

	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	while(1){ // process
		unsigned char buffer[4000] = { 0, };
		retval = recvn(client_sock, reinterpret_cast<char*>(buffer), sizeof(buffer), 0);
		if(retval == SOCKET_ERROR){
			if(WSAGetLastError() != WSAEWOULDBLOCK) {
				err_display("recv()");
				break;
			}
		}

		switch (buffer[1])
		{
		case CS_TYPE_MOVE:
			printf( "move\n" );
			break;
		}

		//else 
		//{

		//	//static int CHAR_SPEED = 1;
		//	
		//	// Get Key Input by Client
		//	if(dir & MOVE_RIGHT){
		//		g_worldData.playerInfo[playerIndex].pos.x++;
		//		if( g_worldData.playerInfo[playerIndex].pos.x > WORLDSIZE)
		//			g_worldData.playerInfo[playerIndex].pos.x = WORLDSIZE;
		//		//printf("RIGHT");
		//	}
		//	if(dir & MOVE_LEFT){
		//		g_worldData.playerInfo[playerIndex].pos.x--;
		//		if( g_worldData.playerInfo[playerIndex].pos.x < 0 )
		//			g_worldData.playerInfo[playerIndex].pos.x = 0;
		//		//printf("LEFT");
		//	}
		//	if(dir & MOVE_UP){
		//		g_worldData.playerInfo[playerIndex].pos.y--;
		//		if( g_worldData.playerInfo[playerIndex].pos.y < 0 )
		//			g_worldData.playerInfo[playerIndex].pos.y = 0;
		//		//printf("UP");
		//	}
		//	if(dir & MOVE_DOWN){
		//		g_worldData.playerInfo[playerIndex].pos.y++;
		//		if( g_worldData.playerInfo[playerIndex].pos.y > WORLDSIZE)
		//			g_worldData.playerInfo[playerIndex].pos.y = WORLDSIZE;
		//		//printf("DOWN");
		//	}
		//}
	}

	/*WaitForSingleObject(hLogInOutEvent, INFINITE);
	ResetEvent(hLogInOutEvent);*/

	printf("client logout\n");

	closesocket(client_sock);
	g_playerSocket = NULL;

	clientAddList[playerIndex].cliensSock = NULL;
	clientAddList[playerIndex].use = false;

	g_worldData.playerInfo[playerIndex].Id = playerIndex;
	g_worldData.playerInfo[playerIndex].login = false;

	return 0;
}

DWORD WINAPI WorldDataBroadCastThread(LPVOID arg)
{	
	int retval;
	int count = 0;
	while( 1 ){
		if( g_playerSocket != NULL )
		{
			// Send Player Position Information to Client
			for( int i = 0; i < 20; i++ ){
				if( clientAddList[i].use ) {
					retval = send( *( clientAddList[i].cliensSock ), (char*)&g_worldData, sizeof( WorldData ), 0 );
					if( retval == SOCKET_ERROR ) {
						if( WSAGetLastError() != WSAEWOULDBLOCK ) {
							err_display( "send()" );
							continue;
						}
					}
				}
			}
			//printf("complete send packet %d\n", count++);
			_sleep( 100 );
		}
	}
	return 0;
}