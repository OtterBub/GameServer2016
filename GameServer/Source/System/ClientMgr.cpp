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
	if (mClientList.count(index) == 1)
	{
		return mClientList[index].is_connected;
	}
	else
		return false;
}

void ClientMgr::DeleteClient(unsigned int index)
{
	if (mClientList.count(index) == 1)
	{
		closesocket(mClientList[index].s);
		mClientList[index].is_connected = false;
		mClientList.erase(index);
	}
}