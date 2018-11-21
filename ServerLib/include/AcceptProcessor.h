#pragma once

class SessionPool;
class NetworkDepartment;

class AcceptProcessor : public IoProcessor
{

	std::atomic<UShort>		postAcceptCount_;
	NetworkDepartment*		networkDept_;

public:
	AcceptProcessor(NetworkDepartment* networkDept);
	~AcceptProcessor();

	Void PreparingAccept(SessionPool* sessionPool,  UShort totalAcceptCount, SOCKET listenSock);
	
	Void CompleteIoEventProcess(Overlapped_Ex* overlapped, Int numberOfTransferred, Bool ioEventResult) override;

};