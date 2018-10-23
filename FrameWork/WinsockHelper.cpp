#include "stdafx.h"

namespace WinsockHelper
{
	SOCKET		CreateTcpSocket()
	{
		SOCKET socket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		if (socket == INVALID_SOCKET)
		{
			std::cout << "CreateTcpSocket()" << std::endl; //임시출력
			return INVALID_SOCKET;
		}

		int sendBufSizeOpt = 0;
		if (::setsockopt(socket, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char*>(&sendBufSizeOpt), sizeof(sendBufSizeOpt)) == SOCKET_ERROR)
		{
			std::cout << "setsockopt()" << std::endl;//임시출력
			::closesocket(socket);
			::WSASetLastError(::WSAGetLastError());
			return INVALID_SOCKET;
		}

		return socket;
	}

	Void		ErrorDisplay(const WString& msg)
	{
		std::wstring errmsg(msg);
		std::wostringstream ostream;

		ostream << errmsg << L" GetLastErrorCode :" << ::WSAGetLastError();
	
		std::wcout << ostream.str().c_str() << std::endl;
		
	}
}



