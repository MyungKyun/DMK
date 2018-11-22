#include "stdafx.h"

ServerNetWorkDepartment::ServerNetWorkDepartment(Iocp* iocp, Dispatcher* dispatcher, SessionPool* sessionPool, const IPv4& address, UShort totalAcceptCount)
	: iocp_(iocp ? iocp : nullptr)
	, sessionPool_(sessionPool ? sessionPool : nullptr)
	, totalAcceptCount_(totalAcceptCount) // 임시
	, address_(address)
	, acceptProcessor_(this)
	, packetDispathcer_(dispatcher)
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

	if (false == sessionPool_->MakeSessionPool(this, totalAcceptCount_))
	{
		return false;
	}

	if (false == listener_.Listen(address_))
	{
		return false;
	}

	iocp_->Resister((HANDLE)listener_.GetListenSocket());

	for (Short i = 0; i < totalAcceptCount_; ++i)
	{
		acceptProcessor_.PreparingAccept(sessionPool_, totalAcceptCount_, listener_.GetListenSocket());
	}

	return true;
}

Void	ServerNetWorkDepartment::AddSession(std::shared_ptr<Session>& session)
{
	{
		READ_LOCK;
		auto found = sessions_.find(session->GetSessionId());
		if (found != sessions_.end())
		{
			//이미 존재하는 세션이다.
			return;
		}
	}

	{
		WRITE_LOCK;
		sessions_.emplace(session->GetSessionId(), std::move(session));
	}
}

Void	ServerNetWorkDepartment::SessionWasDismissed(std::shared_ptr<Session>& sessionPtr)
{
	sessionPool_->SessionReturns(sessionPtr);

	acceptProcessor_.PreparingAccept(sessionPool_, totalAcceptCount_, listener_.GetListenSocket());
}

Void	ServerNetWorkDepartment::RegisterToIocp(HANDLE handle)
{
	iocp_->Resister(handle);
}

Bool	ServerNetWorkDepartment::Dispatch(std::shared_ptr<Session> session, const PacketHeader* header, const Byte* buf, Int transferredBytes)
{
	return packetDispathcer_->Dispatch(session, header, buf, transferredBytes);
}