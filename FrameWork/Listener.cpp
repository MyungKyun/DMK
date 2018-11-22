#include "stdafx.h"

Listener::Listener()
{
	listenSocket_ = WinsockHelper::CreateTcpSocket();
}

Listener::~Listener()
{

}

Bool	Listener::Listen(const IPv4& address)
{
	BOOL reuseAddrOption = TRUE;
	if (::setsockopt(listenSocket_, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuseAddrOption), sizeof(reuseAddrOption)) == SOCKET_ERROR)
	{
		int errorCode = ::WSAGetLastError();
		std::cout << "setsockopt() SO_REUSEADDR failed. errorCode" << std::endl;//임시출력

		::WSASetLastError(errorCode);
		return false;
	}

	//WinsockHelper::NagleOff(listenSocket_);

	if (::bind(listenSocket_, address.GetSockAddr(), address.GetSockAddrLen()) == SOCKET_ERROR)
	{
		int errorCode = ::WSAGetLastError();
		std::cout << "bind() for address(%s:%u) failed. errorCode" << std::endl;//임시출력

		::WSASetLastError(errorCode);
		return false;
	}

	if (::listen(listenSocket_, SOMAXCONN) == SOCKET_ERROR)
	{
		int errorCode = ::WSAGetLastError();
		std::cout << "listen() failed. errorCode" << std::endl; //임시출력

		::WSASetLastError(errorCode);
		return false;
	}

	return true;
}

SOCKET Listener::GetListenSocket()
{
	return listenSocket_;
}