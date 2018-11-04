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
			
	// 패킷큐에 큐잉을 제거해야겠다. IoThread가 패킷처리를 하고 로직을 로직쓰레드 잡으로 넘기는 방안이 좋겠다. 큐잉이 비싸다.
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
		//스타트업 실패
		std::cout << "Failed. WSAStartup" << std::endl;//임시출력
		return false;
	}

	if (LOBYTE(wsa.wVersion) != 2 || HIBYTE(wsa.wVersion) != 2)
	{
		::WSACleanup();
		std::cout << "not found version(2.2) winsock.dll" << std::endl;//임시출력
		return false;
	}
	
	if (false == EXFunction::SetupExFunction())
	{
		return false;
	}

	return true;
}
