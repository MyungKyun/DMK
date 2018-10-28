#include "stdafx.h"
#include "Session.h"

Session::Session(ServerNetWorkDepartment* serverNetDept, Int bufSize, ReceiveProcessor* recvProcessor, SendProcessor* sendProcessor)
	: totalBufferSize_(bufSize)
	, sessionId_(0)
	, recvEnd_(0)
	, recvBegin_(0)
	, serverNetDept_(serverNetDept ? serverNetDept : nullptr)
	, recvProcessor_(recvProcessor ? recvProcessor : nullptr)
	, sendProcessor_(sendProcessor ? sendProcessor : nullptr)

{
	sendBuf_ = new Byte[bufSize];
	recvBuf_ = new Byte[bufSize];

	socket_ = WinsockHelper::CreateTcpSocket();
	std::atomic_init(&completedConnect_, false);
}


Session::~Session()
{
	delete[] sendBuf_;
	delete[] recvBuf_;

	sendBuf_ = nullptr;
	recvBuf_ = nullptr;
}

Void	Session::Disconnect()
{
	
}

Void	Session::SetSessionId(UDLong sessionId)
{
	sessionId_ = sessionId;
}

UDLong	Session::GetSessionId() const
{
	return sessionId_;
}

Int		Session::GetBufferSize() const
{
	return totalBufferSize_;
}

Void	Session::ReRegisterToIocp()
{
	::closesocket(socket_);

	socket_ = WinsockHelper::CreateTcpSocket();

	serverNetDept_->RegisterToIocp(reinterpret_cast<HANDLE>(socket_));

	serverNetDept_->ReturnSessionAndPreparingAccpet(sessionId_);
}


Void	Session::Send(Overlapped_Ex * overlapped, UInt bytes)
{
	
}

Bool	Session::AcceptCompleted(const IPv4& address)
{
	completedConnect_.exchange(true);
	peerAddress_ = address;


	if (false == completedConnect_.load())
	{
		return false;
	}

	if (false == recvProcessor_->ReservingReceive(shared_from_this()))
	{
		return false;
	}
	
	return true;
}

Byte*	Session::GetRecvBuf()
{
	return recvBuf_;
}

Int&	Session::GetRecvBegin()
{
	return recvBegin_;
}

Int&	Session::GetRecvEnd()
{
	return recvEnd_;
}


HANDLE	Session::GetHandle() 
{
	return reinterpret_cast<HANDLE>(socket_);
}

SOCKET	Session::GetSocket()
{
	return socket_;
}



Bool	Session::IsConnected()
{
	if (false == completedConnect_.load())
	{
		return false;
	}

	return true;
}

Void	Session::ResetBufferBeginSize()
{
	recvBegin_ = 0;
}

Void	Session::ResetBufferEndSize()
{
	recvEnd_ = 0;
}
