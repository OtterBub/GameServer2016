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

#include <unordered_map>
#include <map>

#include <unordered_set>
#include <set>

#include <mutex>

#include "Math\Math.h"
#include "..\..\GameServer\Source\PacketStruct.h"
#include "Object\World.h"
#include "Object\Player.h"

// 소켓 함수 오류 출력 후 종료
void err_quit(char* msg);
// 소켓 함수 오류 출력
void err_display(char *msg);

int recvn(SOCKET s, char *buf, int len, int flags);

void drawStrokeText(char *string, float x, float y, float z, float scale);
#endif