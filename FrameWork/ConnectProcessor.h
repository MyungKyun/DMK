#pragma once

class ConnectProcessor : public IoProcessor
{
public:
	
	ConnectProcessor();
	virtual ~ConnectProcessor();

	Bool Connect(std::shared_ptr<Session>& sessionPtr, IPv4& address);

	virtual Void CompleteIoEventProcess(Overlapped_Ex* overlapped, Int numberOfTransferredBytes, Bool ioEventResult) override;
};
