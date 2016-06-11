#pragma once
#ifndef __MTOBJMGR_H__
#define __MTOBJMGR_H__

#include "../RWLock.h"

// for Windows
// RW���� �����ϴ� ��Ƽ������� ��ü ������
template <class key, class T>
class MTObjectMGR
{
public:
	static MTObjectMGR<key, T>& GetInstance();

	T& GetObj(key index);
	bool ExistClient(key index);
	void DeleteClient(key index);

private:
	MTObjectMGR() {}
	MTObjectMGR(const MTObjectMGR& tmp) {}

private:
	std::map<key, T> mObjectList;
	RWLock mRWLock;
};

template <class key, class T>
MTObjectMGR<key, T>& MTObjectMGR<key, T>::GetInstance()
{
	static MTObjectMGR<key, T> instance;

	return instance;
}

template <class key, class T>
T& MTObjectMGR<key, T>::GetObj(key index)
{
	mRWLock.ReadLock();
	T& result = mObjectList[index];
	mRWLock.ReadUnLock();
	return result;
}

template <class key, class T>
bool MTObjectMGR<key, T>::ExistClient(key index)
{
	bool result = false;
	mRWLock.ReadLock();

	result = (mObjectList.count(index) >= 1);

	mRWLock.ReadUnLock();
	return result;
}

template <class key, class T>
void MTObjectMGR<key, T>::DeleteClient(key index)
{
	mRWLock.WriteLock();
	if (mObjectList.count(index) == 1)
	{
		closesocket(mObjectList[index].s);
		mObjectList[index].is_connected = false;
		mObjectList.erase(index);
	}
	mRWLock.WriteUnLock();
}


#endif __MTOBJMGR_H__