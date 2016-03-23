#ifndef __THREAD_H__
#define __THREAD_H__

#include "Common.h"

#define SERVERPORT 9000
#define BUFSIZE 512

static SOCKET sock;

DWORD WINAPI ClientMain(LPVOID arg);

#endif