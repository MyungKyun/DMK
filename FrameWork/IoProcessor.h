#pragma once
class IoProcessor
{
public:
	IoProcessor();
	virtual ~IoProcessor() {}

	virtual Void CompleteIoEventProcess(Overlapped_Ex* overlapped, Int numberOfTransferredBytes, Bool ioEventResult) = 0;
};