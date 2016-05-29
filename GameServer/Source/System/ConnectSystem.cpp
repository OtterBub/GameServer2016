#include "ConnectSystem.h"

void ConnectSystem::WorkerThread()
{
	static int count = 0;
	ConnectSystem lConnect;

	lConnect.mOverlap.operation = count++;
	lConnect.TestFunc();
}
