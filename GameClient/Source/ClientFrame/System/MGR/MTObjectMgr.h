#pragma once
#ifndef __MTOBJMGR_H__
#define __MTOBJMGR_H__

#include "../RWLock.h"

// for Windows
// RW락이 존재하는 멀티스레드용 객체 관리자
template <class key, class T>
class MTObjectMGR
{
public:
	static MTObjectMGR<key, T>& GetInstance(unsigned int index = 0);

	T& GetObj(key index);
	bool ExistClient(key index);
	void DeleteClient(key index);

	const std::map<key, T>& GetList();
	const std::map<key, T> GetCopyList();

	MTObjectMGR() {}
private:
	MTObjectMGR(const MTObjectMGR& tmp) {}

private:
	std::map<key, T> mObjectList;
	RWLock mRWLock;
};

template <class key, class T>
MTObjectMGR<key, T>& MTObjectMGR<key, T>::GetInstance(unsigned int index)
{
	static std::map<unsigned int, MTObjectMGR<key, T>> instance;

	return instance[index];
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
		mObjectList.erase(index);
	}
	mRWLock.WriteUnLock();
}

template <class key, class T>
const std::map<key, T>& MTObjectMGR<key, T>::GetList()
{
	return mObjectList;
}
template <class key, class T>
const std::map<key, T> MTObjectMGR<key, T>::GetCopyList()
{
	return std::map<key, T>(mObjectList);
}

#endif __MTOBJMGR_H__