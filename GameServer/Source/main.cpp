#include "Thread\Thread.h"

int main(int argc, char** argv){
	WSADATA wsa;
	if( WSAStartup( MAKEWORD( 2, 2 ), &wsa ) != 0 )
		return -1;

	HANDLE hThread;
	DWORD ThreadId;

	hThread = CreateThread(NULL, 0, CreatePlayerSocket,
		NULL, 0, &ThreadId);
	if(hThread == NULL)
		printf("[오류] 스레드 생성 실패!\r\n");
	else
		CloseHandle(hThread);

	hThread = CreateThread(NULL, 0, WorldDataBroadCastThread,
		NULL, 0, &ThreadId);
	if(hThread == NULL)
		printf("[오류] 스레드 생성 실패!\r\n");
	else
		CloseHandle(hThread);

	while(1)
	{
		Sleep(0);
	}

	WSACleanup();
	return 0;
}