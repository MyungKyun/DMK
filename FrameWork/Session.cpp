#include "stdafx.h"
#include "Session.h"

Session::Session(NetworkDepartment* serverNetDept, Int bufSize)
	: totalBufferSize_(bufSize)
	, sessionId_(GIDGen.SessionIdGenerate())
	, serverNetDept_(serverNetDept ? serverNetDept : nullptr)
	
{
	socket_ = WinsockHelper::CreateTcpSocket();
	WinsockHelper::NagleOff(socket_);
	std::atomic_init(&completedConnect_, false);
}


Session::~Session()
{
	std::cout << "session dctor call" << std::endl;
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

NetworkDepartment*		Session::GetNetworkDept()
{
	return serverNetDept_;
}

Void	Session::Send(std::shared_ptr<SendBuffer>&& sendBuffer, Int len)
{
	if (false == completedConnect_.load())
	{
		return;
	}

	sendProcessor_.PostSend(shared_from_this(), std::move(sendBuffer), len);
}

Bool	Session::AcceptCompleted(const IPv4& address)
{
	completedConnect_.exchange(true);
	peerAddress_ = address;


	if (false == completedConnect_.load())
	{
		return false;
	}

	WinsockHelper::NagleOff(socket_);

	if (false == recvProcessor_.ReservingReceive(shared_from_this()))
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
