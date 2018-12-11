#pragma once

class ServerNetWorkDepartment;

class Session :	public std::enable_shared_from_this<Session>
{

private:
	
	Session() = delete;
	

private:

	NetworkDepartment*								netDept_;
	UInt64											sessionId_ = 0;
	SOCKET											socket_ = INVALID_SOCKET;
	Int												totalBufferSize_ = 0;
	
	std::atomic_bool								completedConnect_;

	IPv4											peerAddress_;
	ReceiveProcessor								recvProcessor_;
	SendProcessor									sendProcessor_; 

public:
		
	Session(Int ioTotalBufsize);
	~Session();

	Void		Disconnect();
	
	Void		ReRegisterToIocp();
	Void		Send(std::shared_ptr<SendBuffer>&& sendBuffer, Int len);
	Bool		AcceptCompleted(const IPv4& address);
	Bool		ConnectCompleted(const IPv4& address);

	Void		SetNetworkDept(NetworkDepartment* networkDept);


	UInt64		GetSessionId() const;
	Int			GetBufferSize() const;
	HANDLE		GetHandle();
	SOCKET		GetSocket();
	Bool		IsConnected();
	Void		CloseSocket();
	NetworkDepartment*		GetNetworkDept();
	IPv4					GetPeer() const;

	std::shared_ptr<Session>	GetThisPtr();
};