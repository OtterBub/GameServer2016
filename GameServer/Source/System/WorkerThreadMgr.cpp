#include "WorkerThreadMgr.h"

void WorkerThreadMgr::PushThread(std::thread *&& _Val)
{
	mWorkerThreads.push_back(_Val);
}

void WorkerThreadMgr::Join()
{
	for (auto it : mWorkerThreads)
	{
		it->join();
		delete it;
	}
}

void WorkerThreadMgr::Clear()
{
	mWorkerThreads.clear();
}