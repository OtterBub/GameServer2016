#include "RWLock.h"

RWLock::RWLock()
{
	InitializeSRWLock(&mSRWLockHandle);
}
void RWLock::WriteLock()
{
	AcquireSRWLockExclusive(&mSRWLockHandle);
}
void RWLock::WriteUnLock()
{
	ReleaseSRWLockExclusive(&mSRWLockHandle);
}
void RWLock::ReadLock()
{
	AcquireSRWLockShared(&mSRWLockHandle);
}
void RWLock::ReadUnLock()
{
	ReleaseSRWLockShared(&mSRWLockHandle);
}