#include "stdafx.h"


LockSafeScope::LockSafeScope(Lock* lock, LPCWSTR fileName, Int lineNo)
{
	if (lock == nullptr)
	{
		return;
	}

	lock_ = lock;
	
	const Lock* deadLock = LockManager::GetInstance()->CheckDeadLock(lock_);

	if (deadLock != nullptr)
	{
		printf("DeadLock detecting!\n");
		std::lock(lock_->GetMutex(), const_cast<Lock*>(deadLock)->GetMutex());
		return;
	}

	lock_->DoLock(fileName, lineNo);
	lock_->SetThreadId(LThreadId);		// -1 이면 메인스레드이다
}

LockSafeScope::~LockSafeScope()
{
	if (lock_ != nullptr)
	{
		lock_->DoUnlock();
		lock_->SetThreadId(-2);
	}
}   