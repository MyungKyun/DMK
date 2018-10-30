#pragma once


enum IO_TYPE
{
	IO_RESERVING_RECV,
	IO_RECV,
	IO_SEND,
	IO_ACCEPT,
};

class IoProcessor;

struct Overlapped_Ex : public OVERLAPPED
{
	IO_TYPE					ioType_;
	SOCKET					socket_;
	WSABUF					wsabuf_;
	IoProcessor*			processor_;

	Overlapped_Ex(IO_TYPE type, IoProcessor* processor, SOCKET socket, Byte* buf, Int len);
};

struct Overlapped_Ex_Accept : public Overlapped_Ex
{
	Char	buf_[(sizeof(sockaddr_storage) + 16) * 2];
	std::shared_ptr<Session> sessionSptr;


	Overlapped_Ex_Accept(IoProcessor* processor, SOCKET socket, const std::shared_ptr<Session> sessionPtr);
};

struct Overlapped_Ex_Preparing_Receive : public Overlapped_Ex
{
	std::shared_ptr<Session> sessionSPtr;

	Overlapped_Ex_Preparing_Receive(IoProcessor* processor, const std::shared_ptr<Session> sessionPtr);
};

struct Overlapped_Ex_Processing_Receive : public Overlapped_Ex
{
	std::shared_ptr<Session> sessionSPtr;

	Overlapped_Ex_Processing_Receive(IoProcessor* processor, SOCKET socket, Byte* buf, Int len, const std::shared_ptr<Session> sessionPtr);
};

