#include "stdafx.h"

ClientNetWorkDepartment::ClientNetWorkDepartment(Iocp* iocp, SessionPool* sessionPool, const IPv4& address, Int connectCount)
	: iocp_(iocp ? iocp : nullptr)
	, sessionPool_(sessionPool ? sessionPool : nullptr )
	, address_(address)
	, connectCount_(connectCount)
	, connectProcessor_(this)
{

}

ClientNetWorkDepartment::~ClientNetWorkDepartment()
{

}

Bool		ClientNetWorkDepartment::Setup()
{
	if (false == sessionPool_->MakeSessionPool(this, connectCount_))
	{
		return false;
	}

	for (Int i = 0; i < connectCount_; ++i)
	{
		auto& session = sessionPool_->GetSession();
		if (nullptr == session)
		{
			LOG_ERROR(L"Session Is nullptr.");
			return false;
		}
		
		if (false == connectProcessor_.Connect(session, address_))
		{
			return false;
		}
	}

	return true;
}

Void		ClientNetWorkDepartment::RegisterToIocp(HANDLE handle)
{
	BOOL reuseAddrOption = TRUE;
	if (::setsockopt((SOCKET)handle, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuseAddrOption), sizeof(reuseAddrOption)) == SOCKET_ERROR)
	{
		auto error = ::WSAGetLastError();
		LOG_ERROR(L"setsockopt failed. ErrorCode: {}", error);
		::WSASetLastError(error);
		return;
	}

	IPv4 any;
	if (::bind((SOCKET)handle, any.GetSockAddr(), any.GetSockAddrLen()) == SOCKET_ERROR)
	{
		auto error = ::WSAGetLastError();
		LOG_ERROR(L"bind for any IP failed. ErrorCode: {}", error);
		::WSASetLastError(error);
		return;
	}

	iocp_->Resister(handle);
}


Bool		ClientNetWorkDepartment::AddSession(std::shared_ptr<Session>& session)
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

Void		ClientNetWorkDepartment::SessionWasDismissed(std::shared_ptr<Session>& sessionPtr)
{
	sessionPool_->SessionReturns(sessionPtr);
}

Bool		ClientNetWorkDepartment::Dispatch(std::shared_ptr<Session> session, const PacketHeader* header, const Byte* buf, Int transferredBytes)
{
	return dispacher_->Dispatch(session, header, buf, transferredBytes);
}