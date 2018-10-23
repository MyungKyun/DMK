#pragma once

class SessionPool;

class AcceptProcessor : public IoProcessor
{

	std::atomic<UShort>		postAcceptCount_;

public:
	AcceptProcessor();
	~AcceptProcessor();

	Void PreparingAccept(SessionPool* sessionPool,  UShort totalAcceptCount, SOCKET listenSock);
	
	Void CompleteIoEventProcess(Overlapped_Ex* overlapped, Int numberOfTransferred, Bool ioEventResult) override;

};