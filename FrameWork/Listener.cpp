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
		LOG_ERROR(L"setsockopt() SO_REUSEADDR failed. Error:{0}", errorCode);

		::WSASetLastError(errorCode);
		return false;
	}


	if (::bind(listenSocket_, address.GetSockAddr(), address.GetSockAddrLen()) == SOCKET_ERROR)
	{
		int errorCode = ::WSAGetLastError();
		
		LOG_ERROR(L"bind failed. address({0},{1}) Error:{2}", address.GetIpString().c_str(), address.GetPort(), errorCode);

		::WSASetLastError(errorCode);
		return false;
	}

	if (::listen(listenSocket_, SOMAXCONN) == SOCKET_ERROR)
	{
		int errorCode = ::WSAGetLastError();
		LOG_ERROR(L"listen Failed. Error:{0}", errorCode);

		::WSASetLastError(errorCode);
		return false;
	}

	return true;
}

SOCKET Listener::GetListenSocket()
{
	return listenSocket_;
}