#ifndef __THREAD_H__
#define __THREAD_H__

#pragma comment(lib,"ws2_32")

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <thread>
#include <vector>


#include <unordered_map>
#include <map>

#include <unordered_set>
#include <set>

#include <mutex>

#include "PacketStruct.h"

#define BUFSIZE 1024
#define SERVERPOTR 9000
#define WORLDSIZE 100
#define NUM_THREADS 6

// 소켓 함수 오류 출력 후 종료
void err_quit(char* msg);

// 소켓 함수 오류 출력
void err_display(char *msg);

int recvn(SOCKET s, char *buf, int len, int flags);

DWORD WINAPI CreatePlayerSocket(LPVOID arg);
DWORD WINAPI PlayerSoketThread(LPVOID arg);
DWORD WINAPI WorldDataBroadCastThread(LPVOID arg);
extern playerPos g_playerPos;
extern SOCKET g_playerSocket;

#endif 