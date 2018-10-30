#include "stdafx.h"

ServerNetWorkDepartment::ServerNetWorkDepartment(Iocp* iocp, SessionPool* sessionPool, const IPv4& address, UShort totalAcceptCount)
	: iocp_(iocp ? iocp : nullptr)
	, sessionPool_(sessionPool ? sessionPool : nullptr)
	, totalAcceptCount_(totalAcceptCount) // юс╫ц
	, address_(address)
{
	
	
}

ServerNetWorkDepartment::~ServerNetWorkDepartment()
{

}

Bool	ServerNetWorkDepartment::Initialize(PacketProcess* contentLogicProcess)
{
	if (nullptr == iocp_)
	{
		assert(false);
	}

	if (false == sessionPool_->MakeSessionPool(this, totalAcceptCount_, &receiveProcessor_, &sendProcessor_))
	{
		return false;
	}

	receiveProcessor_.Setup(contentLogicProcess);


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

Void	ServerNetWorkDepartment::ReturnSessionAndPreparingAccpet(std::shared_ptr<Session>& sessionPtr)
{
	sessionPool_->SessionReturns(sessionPtr);

	acceptProcessor_.PreparingAccept(sessionPool_, totalAcceptCount_, listener_.GetListenSocket());
}

Void	ServerNetWorkDepartment::RegisterToIocp(HANDLE handle)
{
	iocp_->Resister(handle);
}