#ifndef __COMMON_H__
#define __COMMON_H__

#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <stdlib.h>

#include <iostream>
#include <chrono>
#include <string>

#include <time.h>
#include <glut.h>

#include <thread>
#include <vector>
#include <queue>

#include <unordered_map>
#include <map>

#include <unordered_set>
#include <set>

#include <mutex>

#include "..\..\GameServer\Source\PacketStruct.h"
#include "ClientFrame\System\ConsoleSystem.h"

// ���� �Լ� ���� ��� �� ����
void err_quit(char* msg);
// ���� �Լ� ���� ���
void err_display(char *msg);

int recvn(SOCKET s, char *buf, int len, int flags);
#endif