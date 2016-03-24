#ifndef __COMMON_H__
#define __COMMON_H__

#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <gl\glut.h>

#include "Math\Math.h"
#include "PacketStruct.h"

DWORD WINAPI ClientMain(LPVOID arg);

// 소켓 함수 오류 출력 후 종료
void err_quit(char* msg);
// 소켓 함수 오류 출력
void err_display(char *msg);

int recvn(SOCKET s, char *buf, int len, int flags);
#endif