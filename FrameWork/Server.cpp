#include "stdafx.h"

Server::Server(const WString& name)
	: name_(name)
{
	
}

Server::~Server()
{
	ShutDown();
}

Bool Server::Setup(Int argc, Char* argv[])
{
	if (false == GLogger.Setup(argv[0]))
	{
		return false;
	}

	LOG_INFO(L"========== Logging Start ==========");

	if (false == startup())
	{
		return false;
	}
	
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
		LOG_ERROR(L"WSAStartup Failed.");
		return false;
	}

	if (LOBYTE(wsa.wVersion) != 2 || HIBYTE(wsa.wVersion) != 2)
	{
		::WSACleanup();
		LOG_ERROR(L"Not found version(2.2) winsock.dll");
		return false;
	}
	
	if (false == EXFunction::SetupExFunction())
	{
		return false;
	}

	return true;
}
