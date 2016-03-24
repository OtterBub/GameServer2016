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

// ���� �Լ� ���� ��� �� ����
void err_quit(char* msg);
// ���� �Լ� ���� ���
void err_display(char *msg);

int recvn(SOCKET s, char *buf, int len, int flags);
#endif