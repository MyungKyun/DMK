#include "stdafx.h"

ClientNetWorkDepartment::ClientNetWorkDepartment(Iocp* iocp, SessionPool* sessionPool, const IPv4& address)
	: iocp_(iocp ? iocp : nullptr)
	, sessionPool_(sessionPool ? sessionPool : nullptr )
	, address_(address)
{

}

ClientNetWorkDepartment::~ClientNetWorkDepartment()
{

}

Bool		ClientNetWorkDepartment::Setup()
{

	return true;
}

Void		ClientNetWorkDepartment::RegisterToIocp(HANDLE handle)
{

}


Void		ClientNetWorkDepartment::SessionWasDismissed(const std::shared_ptr<Session>& sessionPtr)
{
	sessionPool_->SessionReturns(sessionPtr);
}