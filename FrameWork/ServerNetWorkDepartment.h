#pragma once


class ServerNetWorkDepartment 
{
	Iocp*					iocp_;
	SessionPool*			sessionPool_;
	IPv4					address_;
	Listener				listener_;
	AcceptProcessor			acceptProcessor_;
	ReceiveProcessor		receiveProcessor_;
	SendProcessor			sendProcessor_;

	UShort					totalAcceptCount_;

public:

	ServerNetWorkDepartment(Iocp* iocp, SessionPool* sessionPool, const IPv4& address, UShort totalAcceptCount);
	~ServerNetWorkDepartment();

	Bool			Initialize(PacketProcess* contentLogicProcess);
	Void			RegisterToIocp(HANDLE handle);
	Void			ReturnSessionAndPreparingAccpet(UDLong sessionId);
};

