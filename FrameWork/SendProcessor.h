#pragma once

class SendProcessor : public IoProcessor
{


public:

	SendProcessor();
	~SendProcessor();

	
	Void	PostSend(const std::shared_ptr<Session>& sessionPtr);
	Void	CompleteIoEventProcess(Overlapped_Ex* overlapped, Int numberOfTransferredBytes, Bool ioEventResult) override;
};
