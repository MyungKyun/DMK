#include "stdafx.h"
#include "Session.h"

Session::Session(NetworkDepartment* serverNetDept, Int bufSize)
	: totalBufferSize_(bufSize)
	, sessionId_(GIDGen.SessionIdGenerate())
	, serverNetDept_(serverNetDept ? serverNetDept : nullptr)
	
{
	socket_ = WinsockHelper::CreateTcpSocket();
	std::atomic_init(&completedConnect_, false);
}


Session::~Session()
{
	::closesocket(socket_);
}

Void	Session::Disconnect()
{
	
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

	serverNetDept_->SessionWasDismissed(GetThisPtr());
}


Void	Session::Send(Byte* buf, Int len)
{
	if (false == completedConnect_.load())
	{
		return;
	}

	sendProcessor_.PostSend(GetThisPtr(), buf, len);
}

Bool	Session::AcceptCompleted(const IPv4& address)
{
	completedConnect_.exchange(true);
	peerAddress_ = address;


	if (false == completedConnect_.load())
	{
		return false;
	}

	if (false == recvProcessor_.ReservingReceive(GetThisPtr()))
	{
		return false;
	}
	
	return true;
}

std::shared_ptr<Session>	Session::GetThisPtr()
{
	return shared_from_this();
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
