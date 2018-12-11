#include "stdafx.h"

ServerNetWorkDepartment::ServerNetWorkDepartment(Iocp* iocp, BaseSessionPool* sessionPool, const IPv4& address, UShort totalAcceptCount)
	: iocp_(iocp ? iocp : nullptr)
	, sessionPool_(sessionPool ? sessionPool : nullptr)
	, totalAcceptCount_(totalAcceptCount) // 임시
	, address_(address)
	, acceptProcessor_(this)
{
	
	
}

ServerNetWorkDepartment::~ServerNetWorkDepartment()
{

}


Bool	ServerNetWorkDepartment::Setup()
{
	if (nullptr == iocp_)
	{
		assert(false);
	}

	if (false == listener_.Listen(address_))
	{
		return false;
	}

	iocp_->Resister((HANDLE)listener_.GetListenSocket());

	for (Short i = 0; i < totalAcceptCount_; ++i)
	{
		auto& sessionPtr = sessionPool_->GetSession();
		if (nullptr == sessionPtr)
		{
			LOG_ERROR(L"Session shared_ptr is nullptr.");
			continue;
		}
		
		RegisterToIocp(sessionPtr->GetHandle());

		acceptProcessor_.PreparingAccept(sessionPtr, totalAcceptCount_, listener_.GetListenSocket());
	}

	return true;
}

Bool	ServerNetWorkDepartment::AddSession(std::shared_ptr<Session>& session)
{
	{
		READ_LOCK;
		auto found = sessions_.find(session->GetSessionId());
		if (found != sessions_.end())
		{
			//이미 존재하는 세션이다.
			return false;
		}
	}

	{
		WRITE_LOCK;
		sessions_.emplace(session->GetSessionId(), std::move(session));
	}

	return true;
}

Void	ServerNetWorkDepartment::SessionWasDismissed(std::shared_ptr<Session>& sessionPtr)
{
	sessionPool_->SessionReturns(sessionPtr);
	auto& sessionSPtr = sessionPool_->GetSession();
	if (nullptr == sessionSPtr)
	{
		LOG_ERROR(L"Session shared_ptr is nullptr.");
		return;
	}

	acceptProcessor_.PreparingAccept(sessionSPtr, totalAcceptCount_, listener_.GetListenSocket());
}

Void	ServerNetWorkDepartment::RegisterToIocp(HANDLE handle)
{
	iocp_->Resister(handle);
}

Bool	ServerNetWorkDepartment::Dispatch(std::shared_ptr<Session> session, const PacketHeader* header, const Byte* buf, Int transferredBytes)
{
	return dispathcer_->Dispatch(session, header, buf, transferredBytes);
}