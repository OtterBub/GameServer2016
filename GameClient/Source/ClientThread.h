#ifndef __THREAD_H__
#define __THREAD_H__

#include "Common.h"
#include "PacketStruct.h"
#include "Object\Player.h"

#define SERVERPORT 9000
#define BUFSIZE 512

extern SOCKET serverSock;
extern Player g_Player;

DWORD WINAPI ClientMain(LPVOID arg);

#endif