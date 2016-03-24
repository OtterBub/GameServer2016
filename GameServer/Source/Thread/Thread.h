#ifndef __THREAD_H__
#define __THREAD_H__

#pragma comment(lib,"ws2_32")

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include "PacketStruct.h"

#define BUFSIZE 1024
#define SERVERPOTR 9000

// ���� �Լ� ���� ��� �� ����
void err_quit(char* msg);

// ���� �Լ� ���� ���
void err_display(char *msg);

int recvn(SOCKET s, char *buf, int len, int flags);

DWORD WINAPI CreatePlayerSocket(LPVOID arg);
DWORD WINAPI PlayerSoketThread(LPVOID arg);
DWORD WINAPI WorldDataBroadCastThread(LPVOID arg);
extern playerPos g_playerPos;
extern SOCKET g_playerSocket;

#endif 