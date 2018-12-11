#include "stdafx.h"

AcceptProcessor::AcceptProcessor(NetworkDepartment* networkDept)
	: postAcceptCount_(0)
	, networkDept_(networkDept)
{

}

AcceptProcessor::~AcceptProcessor()
{

}

Void AcceptProcessor::PreparingAccept(std::shared_ptr<Session>& sessionPtr, UShort totalAcceptCount, SOCKET listenSock)
{
	if (nullptr == sessionPtr)
	{
		LOG_ERROR(L"Session is nullptr");
		return;
	}

	if (postAcceptCount_.fetch_add(1) >= totalAcceptCount)
	{
		postAcceptCount_.fetch_sub(1);
		return;
	}

	sessionPtr->SetNetworkDept(networkDept_);

	Overlapped_Ex_Accept* overlappedAccept = new Overlapped_Ex_Accept(this, listenSock, sessionPtr);

	DWORD recvBytes = 0;
	if (false == EXFunction::AcceptEx(listenSock, sessionPtr->GetSocket(), overlappedAccept->buf_, 0,
		sizeof(sockaddr_storage) + 16, sizeof(sockaddr_storage) + 16, &recvBytes, overlappedAccept))
	{
		auto error = ::WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{
			LOG_ERROR(L"AcceptEx Error:{0}", error);

			delete overlappedAccept;
			overlappedAccept = nullptr;
			return;
		}
	}
}

Void AcceptProcessor::CompleteIoEventProcess(Overlapped_Ex* overlapped, Int numberOfTransferredBytes, Bool ioEventResult)
{
	UNREFERENCED_PARAMETER(numberOfTransferredBytes);
	
	postAcceptCount_.fetch_sub(1);
	
	Overlapped_Ex_Accept* overlappedAccept = static_cast<Overlapped_Ex_Accept*>(overlapped);

	auto& sessionPtr = overlappedAccept->sessionSPtr_;
	SOCKET sessionSocket = overlappedAccept->sessionSPtr_->GetSocket();
	
	Bool result = ioEventResult;
	sockaddr_storage addr;
	socklen_t addrLen = sizeof(addr);

	if (result)
	{
		if (SOCKET_ERROR == ::setsockopt(sessionSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, 
										 reinterpret_cast<Char*>(&overlappedAccept->socket_), sizeof(overlappedAccept->socket_)))
		{
			auto error = ::WSAGetLastError();
			LOG_ERROR(L"AcceptCompleted, setsockopt Error:{0}", error);
			result = false;
		}

		
		if (SOCKET_ERROR == ::getpeername(sessionSocket, reinterpret_cast<sockaddr*>(&addr), &addrLen))
		{
			auto error = ::WSAGetLastError();
			LOG_ERROR(L"acceptCompleted, getpeername Error:{0}", error);
			result = false;
		}
	}

	if (false == result)
	{
		sessionPtr->ReRegisterToIocp();
	}
	else
	{
		if (false == sessionPtr->AcceptCompleted(IPv4(reinterpret_cast<const sockaddr*>(&addr), addrLen)))
		{
			networkDept_->SessionWasDismissed(sessionPtr);
		}
	}


	delete overlappedAccept;
	overlappedAccept = nullptr;
}