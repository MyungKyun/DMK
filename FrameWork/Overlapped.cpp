#include "stdafx.h"


Overlapped_Ex::Overlapped_Ex(IO_TYPE type, IoProcessor* processor, std::shared_ptr<Session> sessionPtr, SOCKET socket, Byte* buf, Int len)
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
	sessionSPtr_ = sessionPtr;
}

Overlapped_Ex_Accept::Overlapped_Ex_Accept(IoProcessor* processor, SOCKET socket, std::shared_ptr<Session> sessionPtr)
	: Overlapped_Ex(IO_ACCEPT, processor, sessionPtr, socket, nullptr, 0)
{

}


Overlapped_Ex_Preparing_Receive::Overlapped_Ex_Preparing_Receive(IoProcessor* processor, std::shared_ptr<Session> sessionPtr)
	: Overlapped_Ex(IO_RESERVING_RECV, processor, sessionPtr, INVALID_SOCKET, nullptr, 0)
{

}

Overlapped_Ex_Processing_Receive::Overlapped_Ex_Processing_Receive(IoProcessor* processor, SOCKET socket, Byte* buf, Int len, std::shared_ptr<Session> sessionPtr)
	: Overlapped_Ex(IO_RECV, processor, sessionPtr, socket, buf, len)
{

}

Overlapped_Ex_Send::Overlapped_Ex_Send(IoProcessor* processor, SOCKET socket, Byte* buf, Int len, Int numberOfSend, std::shared_ptr<Session> sessionPtr)
	: Overlapped_Ex(IO_SEND, processor, sessionPtr, socket, buf, len)
	, numberOfSend_(numberOfSend)
{

}

Overlapped_Ex_Connect::Overlapped_Ex_Connect(IoProcessor* processor, SOCKET socket, IPv4 address, std::shared_ptr<Session> sessionPtr)
	: Overlapped_Ex(IO_CONNECT, processor, sessionPtr, socket, nullptr, 0)
	, address_(address)
{

}