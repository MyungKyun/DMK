#pragma once


class ServerNetWorkDepartment : public NetworkDepartment
{
	Iocp*					iocp_;
	SessionPool*			sessionPool_;
	IPv4					address_;
	Listener				listener_;
	AcceptProcessor			acceptProcessor_;
	Dispatcher*				packetDispathcer_;
	UShort					totalAcceptCount_;

	using ConnectedSessions = std::unordered_map<UInt64, std::shared_ptr<Session>>;
	ConnectedSessions		  sessions_;

	DECLARE_LOCK;

public:

	ServerNetWorkDepartment(Iocp* iocp, Dispatcher* dispatcher, SessionPool* sessionPool, const IPv4& address, UShort totalAcceptCount);
	~ServerNetWorkDepartment();

	Bool			Setup();
	Void			RegisterToIocp(HANDLE handle) final;
	Void			AddSession(std::shared_ptr<Session>& session) final;
	Void			SessionWasDismissed(std::shared_ptr<Session>& sessionPtr) final; // ������ Ǯ�� ��ȯ�ϰ� �ٽ� ���Ʈ ��� 
	Bool			Dispatch(std::shared_ptr<Session> session, const PacketHeader* header, const Byte* buf, Int transferredBytes) override;
};

