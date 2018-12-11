#include "stdafx.h"

ConnectProcessor::ConnectProcessor(NetworkDepartment* networkDept)
	: networkDept_(networkDept)
{

}

ConnectProcessor::~ConnectProcessor()
{

}

Void ConnectProcessor::CompleteIoEventProcess(Overlapped_Ex* overlapped, Int numberOfTransferredBytes, Bool ioEventResult)
{
	Overlapped_Ex_Connect* overlappedConnect = static_cast<Overlapped_Ex_Connect*>(overlapped);
	if (nullptr == overlappedConnect)
	{
		LOG_ERROR(L"ConnectOverlapped is nullptr");
		return;
	}

	auto& sessionPtr = overlappedConnect->sessionSPtr_;
	auto address = overlappedConnect->address_;

	if (false == sessionPtr->ConnectCompleted(address))
	{
		LOG_ERROR(L"ConnectCompleted Failed.");
		return;
	}
	
	
	delete overlappedConnect;
	overlappedConnect = nullptr;
}

Bool ConnectProcessor::Connect(std::shared_ptr<Session>& sessionPtr, IPv4& address)
{
	Overlapped_Ex* overlappedConnect = new Overlapped_Ex_Connect(this, sessionPtr->GetSocket(), address, sessionPtr);

	sessionPtr->SetNetworkDept(networkDept_);

	if (false == EXFunction::ConnectEx(sessionPtr->GetSocket(), address, overlappedConnect))
	{
		auto error = ::WSAGetLastError();
		if (ERROR_IO_PENDING != error)
		{
			LOG_ERROR(L"ConnectEx failed. ErrorCode: {}", error);
			sessionPtr->CloseSocket();

			delete overlappedConnect;
			overlappedConnect = nullptr;

			return false;
		}
	}

	return true;
}