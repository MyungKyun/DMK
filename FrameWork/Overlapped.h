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
	std::shared_ptr<Session> sessionSPtr_;

	Overlapped_Ex(IO_TYPE type, IoProcessor* processor, std::shared_ptr<Session> sessionPtr, SOCKET socket, Byte* buf, Int len);
};

struct Overlapped_Ex_Accept : public Overlapped_Ex
{
	Char	buf_[(sizeof(sockaddr_storage) + 16) * 2];

	Overlapped_Ex_Accept(IoProcessor* processor, SOCKET socket,  std::shared_ptr<Session> sessionPtr);
};

struct Overlapped_Ex_Preparing_Receive : public Overlapped_Ex
{

	Overlapped_Ex_Preparing_Receive(IoProcessor* processor,  std::shared_ptr<Session> sessionPtr);
};

struct Overlapped_Ex_Processing_Receive : public Overlapped_Ex
{
	Overlapped_Ex_Processing_Receive(IoProcessor* processor, SOCKET socket, Byte* buf, Int len,  std::shared_ptr<Session> sessionPtr);
};

struct Overlapped_Ex_Send : public Overlapped_Ex
{
	Overlapped_Ex_Send(IoProcessor* processor, SOCKET socket, Byte* buf, Int len, std::shared_ptr<Session> sessionPtr);
};

