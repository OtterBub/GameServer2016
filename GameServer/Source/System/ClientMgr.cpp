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
	return ( mClientList.count(index) == 1 );
}

void ClientMgr::DeleteClient(unsigned int index)
{
	if (mClientList.count(index) == 1)
	{
		closesocket(mClientList[index].s);
		mClientList.erase(index);
	}
}