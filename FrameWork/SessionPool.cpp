
#include "stdafx.h"

//SessionPool::SessionPool()
//	: poolCount_(0)
//	
//{
//
//}
//
//SessionPool::~SessionPool()
//{
//
//}

//Bool	SessionPool::MakeSessionPool(NetworkDepartment* networkDept, UShort poolCount)
//{
//	if (nullptr == networkDept)
//	{
//		return false;
//	}
//	
//	poolCount_ = poolCount;
//	
//	if (poolCount < 0)
//	{
//		return false;
//	}
//	
//	for (auto i = 0; i < poolCount; ++i)
//	{
//		auto sessionPtr = Session::Create(networkDept, IO_BUFFER_SIZE);
//		sessionQue_.push(sessionPtr);
//		networkDept->RegisterToIocp(sessionPtr->GetHandle());
//	}
//		
//	return true;
//}

//Void	SessionPool::SessionReturns(const std::shared_ptr<Session>& sessionPtr)
//{
//	
//}
//
//SessionSptr	SessionPool::GetSession()
//{
//	
//}
