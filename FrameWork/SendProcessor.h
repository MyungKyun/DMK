#pragma once

class SendProcessor : public IoProcessor
{


public:

	SendProcessor();
	~SendProcessor();

	
	Void	Send();
	Void	CompleteIoEventProcess(Overlapped_Ex* overlapped, Int numberOfTransferredBytes, Bool ioEventResult) override;
};
