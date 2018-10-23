#include "stdafx.h"

Thread::Thread(std::thread* thread, UShort department )
	: thread_(thread)
	, department_(department)
{
	
	threadId_ = ThreadManager::GetInstance()->Gen();

	Init_TLS(threadId_);

	ThreadManager::GetInstance()->PushThread(this); 
}

Thread::~Thread()
{
	
}

UShort	Thread::GetDepartment()
{
	return department_;
}

Void	Thread::SetLock(Lock* lock)
{
	lock_ = lock;
}

const Lock*	Thread::GetLock() const
{
	return lock_;
}

Int Thread::GetId() const
{
	return threadId_;
}