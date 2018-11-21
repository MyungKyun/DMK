#pragma once

class SendProcessor : public IoProcessor
{
	SendBufferQueue									sendBufferQue_;
	Byte*											sendBuf_;
public:

	SendProcessor();
	~SendProcessor();

	
	Void	PostSend(std::shared_ptr<Session> sessionPtr, std::shared_ptr<SendBuffer> sendBuffer, Int len);
	Void	CompleteIoEventProcess(Overlapped_Ex* overlapped, Int numberOfTransferredBytes, Bool ioEventResult) override;
	
private:

	Void postSend(std::shared_ptr<Session> sessionPtr);
};
