#include "stdafx.h"
#include "Session.h"

Session::Session(NetworkDepartment* netDept, Int bufSize)
	: totalBufferSize_(bufSize)
	, sessionId_(GIDGen.SessionIdGenerate())
	, netDept_(netDept ? netDept : nullptr)
	
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

UInt64	Session::GetSessionId() const
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

	netDept_->RegisterToIocp(reinterpret_cast<HANDLE>(socket_));

	netDept_->SessionWasDismissed(GetThisPtr());
}

NetworkDepartment*		Session::GetNetworkDept()
{
	return netDept_;
}

Void	Session::Send(std::shared_ptr<SendBuffer>&& sendBuffer, Int len)
{
	if (false == completedConnect_.load())
	{
		return;
	}

	sendProcessor_.PostSend(shared_from_this(), std::move(sendBuffer), len);
}

IPv4	Session::GetPeer() const
{
	return peerAddress_;
}

Bool	Session::AcceptCompleted(const IPv4& address)
{
	completedConnect_.exchange(true);
	peerAddress_ = address;


	if (false == completedConnect_.load())
	{
		return false;
	}

	//WinsockHelper::NagleOff(socket_);
	LOG_DEBUG(L"Accept complete address: {}", peerAddress_.GetIpString().c_str()); // 테스트후 삭제.
	if (false == recvProcessor_.ReservingReceive(shared_from_this()))
	{
		return false;
	}
	
	return true;
}

Bool	Session::ConnectCompleted(const IPv4& address)
{
	completedConnect_.store(true);
	peerAddress_ = address;

	if (false == netDept_->AddSession(shared_from_this()))
	{
		return false;
	}

	if (false == recvProcessor_.ReservingReceive(shared_from_this()))
	{
		return false;
	}

	LOG_INFO("Connect Complete  address:{0}", address.GetIpChar().c_str()); // 테스트후 삭제.
	return true;
}

Void		Session::CloseSocket()
{
	::closesocket(socket_);
	socket_ = INVALID_SOCKET;
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
