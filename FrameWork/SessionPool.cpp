
#include "stdafx.h"

SessionPool::SessionPool()
	: poolCount_(0)
	, latestSessionId_(0)
{

}

SessionPool::~SessionPool()
{
	if (!sessionQue_.empty())
	{
		for (auto& i : sessionQue_)
		{
			if (i != nullptr)
			{
				i.reset();
			}
		}
		sessionQue_.clear();
	}

	if (!sessionMap_.empty())
	{
		for (auto& i : sessionMap_)
		{
			if (i.second != nullptr)
			{
				i.second.reset();
			}
		}

		sessionMap_.clear();
	}

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
		sessionQue_.push_back(sessionPtr);
		serverNetWorkDept->RegisterToIocp(sessionPtr->GetHandle());
	}
		
	return true;
}

Void	SessionPool::ReturnSession(UDLong sessionId)
{
	LOCK_SCOPE(mutex_);
	
	auto found = sessionMap_.find(sessionId);
	if (found != sessionMap_.cend())
	{
		sessionQue_.emplace_back(std::move(found->second));
	}

	sessionMap_.erase(found);
}

std::shared_ptr<Session>&	SessionPool::Find(UDLong sessionId)
{
	LOCK_SCOPE(mutex_);

	auto found = sessionMap_.find(sessionId);
	if (found == sessionMap_.cend())
	{
		assert(false);
	}

	return found->second;
}

SessionSptr	SessionPool::GetSession()
{
	LOCK_SCOPE(mutex_);

	auto sessionPtr = sessionQue_.front();

	if (nullptr == sessionPtr)
	{
		assert(false);
	}
	
	sessionPtr->SetSessionId(++latestSessionId_);
	sessionMap_.emplace(sessionPtr->GetSessionId(),sessionPtr);
	sessionQue_.pop_front();

	return sessionPtr;
}
