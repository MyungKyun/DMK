#pragma once

class ServerNetWorkDepartment;

class Session :	public std::enable_shared_from_this<Session>
{

private:
	
	Session() = delete;
	Session(ServerNetWorkDepartment* serverNetDept, Int ioTotalBufsize, ReceiveProcessor* recvProcessor, SendProcessor* sendProcessor);


private:

	ServerNetWorkDepartment*		serverNetDept_;
	UDLong							sessionId_ = 0;
	SOCKET							socket_ = INVALID_SOCKET;
	Int								totalBufferSize_ = 0;

	Byte*							recvBuf_;
	Byte*							sendBuf_;

	Int								recvEnd_, recvBegin_;

	UInt							processedBytes_ = 0;
	UInt							receivedBytes_ = 0;
	std::atomic_bool				completedConnect_;

	IPv4							peerAddress_;
	ReceiveProcessor*				recvProcessor_;
	SendProcessor*					sendProcessor_; // 리시브, 센드를,, 그냥 세션이 가지고 있을까.. ServerNetDepartment에서 옮겨오는걸 고려해보자.

public:

	template <typename ... Args>
	static std::shared_ptr<Session> CreateSession(Args&&... args)
	{
		return  std::shared_ptr<Session>(new Session(std::forward<Args>(args)...));
	}

	
	~Session();


	Void		Disconnect();
	

	Void		SetSessionId(UDLong sessionId);
	
	Void		ReRegisterToIocp();
	Void		Send(Overlapped_Ex* overlapped, UInt bytes);
	Bool		AcceptCompleted(const IPv4& address);


	Byte*		GetRecvBuf();
	Int&		GetRecvBegin();
	Int&		GetRecvEnd();
	UDLong		GetSessionId() const;
	Int			GetBufferSize() const;
	HANDLE		GetHandle();
	SOCKET		GetSocket();
	Bool		IsConnected();
	Void		ResetBufferBeginSize();
	Void		ResetBufferEndSize();

};