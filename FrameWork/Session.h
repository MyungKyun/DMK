#pragma once

class ServerNetWorkDepartment;

class Session :	public std::enable_shared_from_this<Session>
{

private:
	
	Session() = delete;
	Session(NetworkDepartment* serverNetDept, Int ioTotalBufsize);


private:

	NetworkDepartment*								serverNetDept_;
	UInt64											sessionId_ = 0;
	SOCKET											socket_ = INVALID_SOCKET;
	Int												totalBufferSize_ = 0;
	
	std::atomic_bool								completedConnect_;

	IPv4											peerAddress_;
	ReceiveProcessor								recvProcessor_;
	SendProcessor									sendProcessor_; 

public:

	template <typename ... Args>
	static std::shared_ptr<Session> CreateSession(Args&&... args)
	{
		return  std::shared_ptr<Session>(new Session(std::forward<Args>(args)...));
	}
	
	~Session();

	Void		Disconnect();
	
	Void		ReRegisterToIocp();
	Void		Send(std::shared_ptr<SendBuffer>&& sendBuffer, Int len);
	Bool		AcceptCompleted(const IPv4& address);
	
	UInt64		GetSessionId() const;
	Int			GetBufferSize() const;
	HANDLE		GetHandle();
	SOCKET		GetSocket();
	Bool		IsConnected();
	
	NetworkDepartment*		GetNetworkDept();


	std::shared_ptr<Session>	GetThisPtr();
};