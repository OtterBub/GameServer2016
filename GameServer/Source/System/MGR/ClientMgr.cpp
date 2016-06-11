#include "ClientMgr.h"

//ClientMgr& ClientMgr::GetInstance()
//{
//	static ClientMgr instance;
//
//	return instance;
//}
//
//Client& ClientMgr::GetClient(unsigned int index)
//{
//	mRWLock.ReadLock();
//	Client& result = mClientList[index];
//	mRWLock.ReadUnLock();
//	return result;
//}
//
//bool ClientMgr::ExistClient(unsigned int index)
//{
//	bool result = false;
//	mRWLock.ReadLock();
//	if (mClientList.count(index) == 1)
//	{
//		result = mClientList[index].is_connected;
//	}
//	else
//	{
//		result = false;
//	}
//	mRWLock.ReadUnLock();
//	return result;
//}
//
//void ClientMgr::DeleteClient(unsigned int index)
//{
//	mRWLock.WriteLock();
//	if (mClientList.count(index) == 1)
//	{
//		closesocket(mClientList[index].s);
//		mClientList[index].is_connected = false;
//		mClientList.erase(index);
//	}
//	mRWLock.WriteUnLock();
//}