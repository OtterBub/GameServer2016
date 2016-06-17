#ifndef __THREAD_H__
#define __THREAD_H__

#pragma comment(lib,"ws2_32")

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#include <windows.h>
#include <sqlext.h>
#include <string>

#include <iostream>
#include <thread>
#include <vector>


#include <unordered_map>
#include <map>

#include <unordered_set>
#include <set>

#include <mutex>
#include <queue>

// custum header
#include "PacketStruct.h"
#include "Global.h"

#define BUFSIZE 1024
#define SERVERPOTR 9000
#define WORLDSIZE 100
#define NUM_THREADS 6

extern playerPos g_playerPos;
extern SOCKET g_playerSocket;

#endif 