#pragma once
#ifndef __RWLOCK_H__
#define __RWLOCK_H__

#include "../Common.h"

class RWLock
{
public:
	RWLock(); //Init;
	void WriteLock();
	void WriteUnLock();
	void ReadLock();
	void ReadUnLock();

private:
	RWLock(const RWLock& tmp) {};

private:
	SRWLOCK mSRWLockHandle;
};

#endif 