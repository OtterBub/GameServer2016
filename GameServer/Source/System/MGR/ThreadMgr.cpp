#include "ThreadMgr.h"

void ThreadMgr::PushThread(std::thread *&& _Val)
{
	mWorkerThreads.push_back(_Val);
}

void ThreadMgr::Join()
{
	for (auto it : mWorkerThreads)
	{
		it->join();
		delete it;
	}
}

void ThreadMgr::Clear()
{
	mWorkerThreads.clear();
}