#pragma once

class NetworkDepartment 
{

public:

	NetworkDepartment();
	virtual ~NetworkDepartment();

	virtual Void	RegisterToIocp(HANDLE handle) = 0;
	virtual	Void	SessionWasDismissed(const std::shared_ptr<Session>& sessionPtr) = 0; // �̸��� ����.. �ָ��ϴ�..
};
