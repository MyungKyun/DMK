#pragma once

class EXFunction 
{
	

	static LPFN_ACCEPTEX		acceptExFunction_;
	static LPFN_CONNECTEX		connectExFunction_;

public:

	
	~EXFunction() {};

	static Bool			AcceptEx(SOCKET listen, SOCKET accept, PVOID outBuffer, DWORD recvDataLen, DWORD addrLen,
							DWORD remoteAddrLen, LPDWORD byteRecv, LPOVERLAPPED overlapped);

	static Bool			SetupExFunction();

	
};
