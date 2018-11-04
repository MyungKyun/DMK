#pragma once


class ServerNetWorkDepartment : public NetworkDepartment
{
	Iocp*					iocp_;
	SessionPool*			sessionPool_;
	IPv4					address_;
	Listener				listener_;
	AcceptProcessor			acceptProcessor_;
	
	UShort					totalAcceptCount_;

public:

	ServerNetWorkDepartment(Iocp* iocp, SessionPool* sessionPool, const IPv4& address, UShort totalAcceptCount);
	~ServerNetWorkDepartment();

	Bool			Setup();
	Void			RegisterToIocp(HANDLE handle) final;

	Void			SessionWasDismissed(const std::shared_ptr<Session>& sessionPtr) final; // 세션을 풀에 반환하고 다시 억셉트 대기 
	
};

