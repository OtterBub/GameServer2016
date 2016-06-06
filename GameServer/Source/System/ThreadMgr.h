#ifndef __WORKERTHREAD_MGR_H__
#define __WORKERTHREAD_MGR_H__
#include "../Common.h"


class ThreadMgr
{
public:
	void PushThread(std::thread *&&_Val);
	void Join();
	void Clear();

private:
	std::vector<std::thread*> mWorkerThreads;
};


#endif

