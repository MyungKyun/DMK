#pragma once

class NetworkDepartment;
class ConnectProcessor : public IoProcessor
{
	NetworkDepartment*		networkDept_;

public:
	
	ConnectProcessor(NetworkDepartment* networkDept);
	virtual ~ConnectProcessor();

	Bool Connect(std::shared_ptr<Session>& sessionPtr, IPv4& address);

	virtual Void CompleteIoEventProcess(Overlapped_Ex* overlapped, Int numberOfTransferredBytes, Bool ioEventResult) override;
};
