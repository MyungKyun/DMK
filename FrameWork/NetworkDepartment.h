#pragma once

class NetworkDepartment 
{

public:

	NetworkDepartment();
	virtual ~NetworkDepartment();

	virtual Bool	Setup() { return true; }
	virtual Void	AddSession(std::shared_ptr<Session>& session) = 0;
	virtual Void	RegisterToIocp(HANDLE handle) = 0;
	virtual	Void	SessionWasDismissed(std::shared_ptr<Session>& sessionPtr) = 0; // �̸��� ����.. �ָ��ϴ�..
	virtual Bool	Dispatch(std::shared_ptr<Session> session, const PacketHeader* header, const Byte* buf, Int transferredBytes) { return true; }
};
