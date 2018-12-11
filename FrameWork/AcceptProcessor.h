#pragma once

class BaseSessionPool;
class NetworkDepartment;

class AcceptProcessor : public IoProcessor
{

	std::atomic<UShort>		postAcceptCount_;
	NetworkDepartment*		networkDept_;

public:
	AcceptProcessor(NetworkDepartment* networkDept);
	~AcceptProcessor();

	Void PreparingAccept(std::shared_ptr<Session>& sessionPool,  UShort totalAcceptCount, SOCKET listenSock);
	
	Void CompleteIoEventProcess(Overlapped_Ex* overlapped, Int numberOfTransferred, Bool ioEventResult) override;

};