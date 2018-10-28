#pragma once

class PacketProcess
{
	enum Count
	{
		MAX_THREAD_COUNT = 64,
	};
	
	std::array<Thread*, Count::MAX_THREAD_COUNT> threads_;
	
	concurrency::concurrent_queue<DataPack*> packetQue_;
	
	DWORD startThreadCount_;

	std::atomic_int		curCount_;


protected:

	using PacketHandle = Void(*)(Byte* buffer, Int bytes, std::shared_ptr<Session> sessionPtr);
	std::unordered_map < UShort, PacketHandle> handlerTable_;

public:

	PacketProcess();
	
	~PacketProcess();

	Bool Setup();

	Void Run();

	Void Push(DataPack* packetData);

	virtual Void RegisterHandler() = 0;

private:

	Void execute();
};
