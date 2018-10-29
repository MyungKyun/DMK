#include "stdafx.h"

Lock::Lock()
{
	LockfileName_ = L"";
	LockLine_ = -1;
	threadId_ = -1;
//	lockId_ = LockManager::GetInstance()->GenId();
}

Lock::~Lock()
{
	threadId_ = -1;
	lockId_ = -1;
}

Void Lock::DoLock(LPCWSTR fileName, Int lineNo)
{
	mtx_.lock();

	LockfileName_ = fileName;
	LockLine_ = lineNo;
}

Void Lock::DoUnlock()
{
	mtx_.unlock();

	LockfileName_.clear();
	LockLine_ = -1;
}

UDLong Lock::GetLockId() const
{
	return lockId_;
}

Lock_t& Lock::GetMutex()
{
	return mtx_;
}

Void Lock::SetThreadId(Int id)
{
	threadId_ = id;
}

Int Lock::GetThreadId() const
{
	return threadId_;
}

