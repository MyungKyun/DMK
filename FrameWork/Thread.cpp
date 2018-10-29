#include "stdafx.h"

Thread::Thread(const String& threadName, Short deptNum)
	: name_(threadName)
	, deptNumber_(deptNum)
{

}

Thread::~Thread()
{
	isStop_ = true;
	if (thread_.joinable())
	{
		thread_.join();
	}
}

Short	Thread::GetDepartmentNumber()
{
	return deptNumber_;
}


Void	Thread::Initialize()
{
	thread_ = std::thread(&Thread::ThreadRun, this);
}

Bool	Thread::Setup()
{
	::Init_TLS(GetCurrentThreadId(), deptNumber_);
	::SetThreadName(LThreadId, name_.c_str());

	return true;
}

Void	Thread::ThreadRun(Thread* thread)
{
	assert(thread);

	if (thread->Setup())
	{
		thread->Run();
	}

	thread->ShutDown();
}