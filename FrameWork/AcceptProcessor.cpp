#include "stdafx.h"

AcceptProcessor::AcceptProcessor()
	: postAcceptCount_(0)
{

}

AcceptProcessor::~AcceptProcessor()
{

}

Void AcceptProcessor::PreparingAccept(SessionPool* sessionPool, UShort totalAcceptCount, SOCKET listenSock)
{
	if (nullptr == sessionPool)
	{
		WinsockHelper::ErrorDisplay(L"PreparingAccept(), sessionPool was Nullptr!");//임시출력
		return;
	}

	if (postAcceptCount_.fetch_add(1) >= totalAcceptCount)
	{
		postAcceptCount_.fetch_sub(1);
		return;
	}

	decltype(auto) sessionPtr = sessionPool->GetSession();

	Overlapped_Ex_Accept* overlappedAccept = new Overlapped_Ex_Accept(this, listenSock, sessionPtr);

	DWORD recvBytes = 0;
	if (false == EXFunction::AcceptEx(listenSock, sessionPtr->GetSocket(), overlappedAccept->buf_, 0,
		sizeof(sockaddr_storage) + 16, sizeof(sockaddr_storage) + 16, &recvBytes, overlappedAccept))
	{
		auto error = ::WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{
			WinsockHelper::ErrorDisplay(L"AcceptEx error");//임시출력

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

	auto& sessionPtr = overlappedAccept->sessionSptr;
	SOCKET sessionSocket = overlappedAccept->sessionSptr->GetSocket();
	
	Bool result = ioEventResult;
	sockaddr_storage addr;
	socklen_t addrLen = sizeof(addr);

	if (result)
	{
		if (SOCKET_ERROR == ::setsockopt(sessionSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, 
										 reinterpret_cast<Char*>(&overlappedAccept->socket_), sizeof(overlappedAccept->socket_)))
		{
			auto error = ::WSAGetLastError();
			WinsockHelper::ErrorDisplay(L"acceptCompleted, setsockopt error");//임시출력
			result = false;
		}

		
		if (SOCKET_ERROR == ::getpeername(sessionSocket, reinterpret_cast<sockaddr*>(&addr), &addrLen))
		{
			auto error = ::WSAGetLastError();
			WinsockHelper::ErrorDisplay(L"acceptCompleted, getpeername error");//임시출력
			result = false;
		}
	}

	if (false == result)
	{
		sessionPtr->ReRegisterToIocp();
	}
	else
	{
		sessionPtr->AcceptCompleted(IPv4(reinterpret_cast<const sockaddr*>(&addr), addrLen));
	}


	delete overlappedAccept;
	overlappedAccept = nullptr;
}