#include "stdafx.h"

LPFN_ACCEPTEX	EXFunction::acceptExFunction_ = nullptr;
LPFN_CONNECTEX	EXFunction::connectExFunction_ = nullptr;


Bool	EXFunction::AcceptEx(SOCKET listen, SOCKET accept, PVOID outBuffer, DWORD recvDataLen, DWORD addrLen, DWORD remoteAddrLen, LPDWORD byteRecv, LPOVERLAPPED overlapped)
{
	if (nullptr == acceptExFunction_)
	{
		return false;
	}

	if (false == acceptExFunction_(listen, accept, outBuffer, recvDataLen, addrLen, remoteAddrLen, byteRecv, overlapped))
	{
		return false;
	}

	return true;
}


Bool EXFunction::SetupExFunction()
{

	SOCKET socketDummy = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (nullptr == acceptExFunction_)
	{
		DWORD bytes = 0;
		GUID  acceptExGuid = WSAID_ACCEPTEX;
		if (::WSAIoctl(socketDummy, SIO_GET_EXTENSION_FUNCTION_POINTER, &acceptExGuid, sizeof(acceptExGuid),
			&acceptExFunction_, sizeof(acceptExFunction_), &bytes, nullptr, nullptr) == SOCKET_ERROR)
		{
			std::cout << "WSAIoctl() AcceptEx Initialize failed."<< std::endl;//임시출력
			return false;
		}
	}

	return true;
}



