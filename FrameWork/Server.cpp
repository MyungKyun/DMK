#include "stdafx.h"

Server::Server(PacketProcess* contentLogicProcess)
	: contentLogicProcess_(contentLogicProcess)
	
{
	
}

Server::~Server()
{
	ShutDown();

	delete contentLogicProcess_;
	contentLogicProcess_ = nullptr;
}

Bool Server::Setup(SessionPool* sessionPool)
{
	if (false == startup())
	{
		return false;
	}

	if (false == netDeptManger_.MakeDeaprtment<ServerNetWorkDepartment>(&iocp_, sessionPool, IPv4("127.0.0.1", 20000), Count::TOTAL_ACCEPT_COUNT))
	{
		return false;
	}
			
	// ��Ŷť�� ť���� �����ؾ߰ڴ�. IoThread�� ��Ŷó���� �ϰ� ������ ���������� ������ �ѱ�� ����� ���ڴ�. ť���� ��δ�.
	/*if (false == contentLogicProcess_->Setup())
	{
		return false;
	}*/

	return true;
}

Void Server::Run()
{
	
}

Void Server::ShutDown()
{
	::WSACleanup();

}

Bool Server::startup()
{
	WSADATA wsa;
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		//��ŸƮ�� ����
		std::cout << "Failed. WSAStartup" << std::endl;//�ӽ����
		return false;
	}

	if (LOBYTE(wsa.wVersion) != 2 || HIBYTE(wsa.wVersion) != 2)
	{
		::WSACleanup();
		std::cout << "not found version(2.2) winsock.dll" << std::endl;//�ӽ����
		return false;
	}
	
	if (false == EXFunction::SetupExFunction())
	{
		return false;
	}

	return true;
}
