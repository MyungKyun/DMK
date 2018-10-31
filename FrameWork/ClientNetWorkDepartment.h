#pragma once

class ClientNetWorkDepartment : public NetworkDepartment
{

	Iocp*					iocp_;
	SessionPool*			sessionPool_;
	IPv4					address_;

public:

	ClientNetWorkDepartment(Iocp* iocp, SessionPool* sessionPool, const IPv4& adress);
	~ClientNetWorkDepartment();

	Bool		Setup();
	Void		RegisterToIocp(HANDLE handle) final;
	Void		SessionWasDismissed(const std::shared_ptr<Session>& sessionPtr) final;
};
