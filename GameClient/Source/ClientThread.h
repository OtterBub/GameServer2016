#ifndef __THREAD_H__
#define __THREAD_H__

#include "Common.h"
#include "PacketStruct.h"
#include "Object\Player.h"

#define SERVERPORT 4000
#define BUFSIZE 512

extern SOCKET serverSock;
extern Player g_Player[20];
extern WorldData g_worldData;
extern int playerID;

DWORD WINAPI ClientMain(LPVOID arg);

#endif