#pragma once

class ReceiveProcessor	: public IoProcessor
{

	PacketProcess*			contentsLogicProcess_;
	std::atomic_bool		needNotify_;


public:
	ReceiveProcessor();
	~ReceiveProcessor();

	Void	Setup(PacketProcess* contentLogicProcess);

	Void	CompleteIoEventProcess(Overlapped_Ex* overlapped, Int numberOfTransferredBytes, Bool ioEventResult) override;

	Bool	ReservingReceive(const std::shared_ptr<Session>& sessionPtr);

private:

	Void	preparingReceive(Overlapped_Ex* overlapped, Int numberOfTransferredBytes);
	Void	postReceive(Overlapped_Ex* overlapped, Int numberOfTransferredBytes);
	Void	processingReceive(Overlapped_Ex* overlapped, Int numberOfTransferredBytes);
	
};
