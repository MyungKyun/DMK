#include "stdafx.h"

Server::Server(const WString& name)
	: name_(name)
{
	
}

Server::~Server()
{
	ShutDown();
}

Bool Server::Setup()
{
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
		//��ŸƮ�� ����
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
