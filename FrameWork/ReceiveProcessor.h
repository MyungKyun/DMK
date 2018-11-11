#pragma once

class ReceiveProcessor	: public IoProcessor
{

	std::atomic_bool		needNotify_;
	Byte*					recvBuf_;
	Int						recvEnd_, recvBegin_;
	Int						totalRecvBufferSize_;

public:

	ReceiveProcessor();
	~ReceiveProcessor();

	Void	CompleteIoEventProcess(Overlapped_Ex* overlapped, Int numberOfTransferredBytes, Bool ioEventResult) override;
	Bool	ReservingReceive(std::shared_ptr<Session> sessionPtr);

private:

	Void	preparingReceive(Overlapped_Ex* overlapped, Int numberOfTransferredBytes);
	Void	postReceive(Overlapped_Ex* overlapped, Int numberOfTransferredBytes);
	Void	processingReceive(Overlapped_Ex* overlapped, Int numberOfTransferredBytes);
	
};
