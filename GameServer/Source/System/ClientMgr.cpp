#include "ClientMgr.h"

ClientMgr& ClientMgr::GetInstance()
{
	static ClientMgr instance;

	return instance;
}

Client& ClientMgr::GetClient(unsigned int index)
{
	return mClientList[index];
}

bool ClientMgr::ExistClient(unsigned int index)
{
	bool result = false;
	mLock.lock();
	if (mClientList.count(index) == 1)
	{
		result = mClientList[index].is_connected;
	}
	else
	{
		result = false;
	}
	mLock.unlock();
	return result;
}

void ClientMgr::DeleteClient(unsigned int index)
{
	mLock.lock();
	if (mClientList.count(index) == 1)
	{
		closesocket(mClientList[index].s);
		mClientList[index].is_connected = false;
		mClientList.erase(index);
	}
	mLock.unlock();
}