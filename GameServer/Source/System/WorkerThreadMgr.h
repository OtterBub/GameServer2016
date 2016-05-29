#ifndef __WORKERTHREAD_MGR_H__
#define __WORKERTHREAD_MGR_H__
#include "../Thread/Thread.h"


class WorkerThreadMgr
{
public:
	void PushThread(std::thread *&&_Val);
	void Join();
	void Clear();

private:
	std::vector<std::thread*> mWorkerThreads;
};


#endif

