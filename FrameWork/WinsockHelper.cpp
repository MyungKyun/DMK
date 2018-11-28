#include "stdafx.h"

namespace WinsockHelper
{
	

	Void		ErrorDisplay(const WString& msg)
	{
		std::wstring errmsg(msg);
		std::wostringstream ostream;

		ostream << errmsg << L" GetLastErrorCode :" << ::WSAGetLastError();
	
		std::wcout << ostream.str().c_str() << std::endl;
		
	}

	SOCKET		CreateTcpSocket()
	{
		SOCKET socket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		if (socket == INVALID_SOCKET)
		{
			LOG_ERROR(L"WSASocket Failed. Error:{0}", ::WSAGetLastError());
			return INVALID_SOCKET;
		}

		int sendBufSizeOpt = 0;
		if (::setsockopt(socket, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char*>(&sendBufSizeOpt), sizeof(sendBufSizeOpt)) == SOCKET_ERROR)
		{
			LOG_ERROR(L"setsockopt SO_SNDBUF Falied. Error:{0}", ::WSAGetLastError());
			::closesocket(socket);
			::WSASetLastError(::WSAGetLastError());
			return INVALID_SOCKET;
		}

		return socket;
	}

	Void        NagleOff(SOCKET socket)
	{
		Int optVal = 1;
		if (SOCKET_ERROR == ::setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&optVal), sizeof(optVal)))
		{
			LOG_ERROR(L"NagleOff Failed. Error{0}", ::WSAGetLastError());
		}
	}
}





