#include "stdafx.h"


Overlapped_Ex::Overlapped_Ex(IO_TYPE type, IoProcessor* processor, SOCKET socket, Byte* buf, Int len)
{
	OVERLAPPED::hEvent = 0;
	OVERLAPPED::Internal = 0;
	OVERLAPPED::InternalHigh = 0;
	OVERLAPPED::Offset = 0;
	OVERLAPPED::Pointer = 0;
	OVERLAPPED::OffsetHigh = 0;

	ioType_ = type;
	socket_ = socket;
	wsabuf_.buf = reinterpret_cast<char*>(buf);
	wsabuf_.len = len;
	processor_ = processor;
}

Overlapped_Ex_Accept::Overlapped_Ex_Accept(IoProcessor* processor, SOCKET socket, const std::shared_ptr<Session>& sessionPtr)
	: Overlapped_Ex(IO_ACCEPT, processor, socket, nullptr, 0)
	, sessionSptr(sessionPtr)
{

}


Overlapped_Ex_Preparing_Receive::Overlapped_Ex_Preparing_Receive(IoProcessor* processor, const std::shared_ptr<Session> sessionPtr)
	: Overlapped_Ex(IO_RESERVING_RECV, processor, INVALID_SOCKET, nullptr, 0)
	, sessionSPtr(sessionPtr)
{

}

Overlapped_Ex_Processing_Receive::Overlapped_Ex_Processing_Receive(IoProcessor* processor, SOCKET socket, Byte* buf, Int len, const std::shared_ptr<Session> sessionPtr)
	: Overlapped_Ex(IO_RECV, processor, socket, buf, len)
	, sessionSPtr(sessionPtr)
{

}