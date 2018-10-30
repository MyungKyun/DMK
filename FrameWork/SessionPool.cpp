
#include "stdafx.h"

SessionPool::SessionPool()
	: poolCount_(0)
	
{

}

SessionPool::~SessionPool()
{

}

Bool	SessionPool::MakeSessionPool( ServerNetWorkDepartment* serverNetWorkDept, UShort poolCount, ReceiveProcessor* recvProcessor, SendProcessor* sendProcessor)
{
	if (nullptr == serverNetWorkDept)
	{
		return false;
	}
	
	poolCount_ = poolCount;
	
	if (poolCount < 0)
	{
		return false;
	}
	
	for (auto i = 0; i < poolCount; ++i)
	{
		auto sessionPtr = Session::CreateSession(serverNetWorkDept, IO_BUFFER_SIZE, recvProcessor, sendProcessor);
		sessionQue_.push(sessionPtr);
		serverNetWorkDept->RegisterToIocp(sessionPtr->GetHandle());
	}
		
	return true;
}

Void	SessionPool::SessionReturns(std::shared_ptr<Session>& sessionPtr)
{
	sessionQue_.push(sessionPtr);
}

SessionSptr	SessionPool::GetSession()
{
	std::shared_ptr<Session> sessionPtr;
	if (sessionQue_.try_pop(sessionPtr))
	{
		return  sessionPtr;
	}
	
	return nullptr;
}
