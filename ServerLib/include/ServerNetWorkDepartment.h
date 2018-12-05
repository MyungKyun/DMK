#pragma once

class Session;
class ServerNetWorkDepartment : public NetworkDepartment
{
	Iocp*					iocp_;
	SessionPool*			sessionPool_;
	IPv4					address_;
	Listener				listener_;
	AcceptProcessor			acceptProcessor_;
	Dispatcher*				dispathcer_;
	UShort					totalAcceptCount_;

	using ConnectedSessions = std::unordered_map<UInt64, std::shared_ptr<Session>>;
	ConnectedSessions		  sessions_;

	DECLARE_LOCK;

public:

	ServerNetWorkDepartment(Iocp* iocp, SessionPool* sessionPool, const IPv4& address, UShort totalAcceptCount);
	~ServerNetWorkDepartment();

	Bool			Setup() override;
	Void			RegisterToIocp(HANDLE handle) final;
	Bool			AddSession(std::shared_ptr<Session>& session) final;
	Void			SessionWasDismissed(std::shared_ptr<Session>& sessionPtr) final; // 세션을 풀에 반환하고 다시 억셉트 대기 
	Bool			Dispatch(std::shared_ptr<Session> session, const PacketHeader* header, const Byte* buf, Int transferredBytes) override;
};

