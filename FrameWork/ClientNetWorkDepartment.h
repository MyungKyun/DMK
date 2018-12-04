#pragma once

class ClientNetWorkDepartment : public NetworkDepartment
{

	Iocp*					iocp_;
	SessionPool*			sessionPool_;
	IPv4					address_;
	Dispatcher*				dispacher_;
	Int						connectCount_;
	ConnectProcessor		connectProcessor_;

	using ConnectedSessions = std::unordered_map<UInt64, std::shared_ptr<Session>>;
	ConnectedSessions		  sessions_;

public:

	ClientNetWorkDepartment(Iocp* iocp, SessionPool* sessionPool, const IPv4& adress, Int connectCount);
	~ClientNetWorkDepartment();

	Bool		Setup() override;
	Void		RegisterToIocp(HANDLE handle) final;
	Void		AddSession(std::shared_ptr<Session>& session) final;
	Void		SessionWasDismissed(std::shared_ptr<Session>& sessionPtr) final;
	Bool		Dispatch(std::shared_ptr<Session> session, const PacketHeader* header, const Byte* buf, Int transferredBytes) override;
};
