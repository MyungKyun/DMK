#include "stdafx.h"

PacketProcess::PacketProcess()
	: startThreadCount_(0)
{
	
}

PacketProcess::~PacketProcess()
{
	packetQue_.clear();
	
	for (int i = 0; i < threads_.size(); ++i)
	{
		delete threads_[i];
		threads_[i] = nullptr;
	}

	handlerTable_.clear();
}

Bool PacketProcess::Setup()
{
	// 임시
	if (startThreadCount_ <= 0)
	{
		SYSTEM_INFO sys;
		::GetSystemInfo(&sys);

		startThreadCount_ = 4;
	}

	for (DWORD i = 0; i < startThreadCount_; ++i)
	{
		threads_[i] = _CREATE_THREAD(PacketProcess, Run, PACKET_PROCESSING_DEPT);
	}

	return true;
}

Void PacketProcess::Push(DataPack* packetData)
{
	packetQue_.push(packetData);
}

Void PacketProcess::Run()
{
	while (true)
	{
		execute();
	}
}

Void PacketProcess::execute()
{
	DataPack* pack = nullptr;

	if (packetQue_.empty())
	{
		std::mutex mtx;
		std::unique_lock<std::mutex> lock(mtx);
		GisEmptyPacketQueue_.exchange(true);
		GhasPacketDataCond_.wait(lock);	// 큐에 패킷이 하나라도 들어오면 모든 스레드가 깨어난다.
	}

	if (false == packetQue_.try_pop(pack))
	{
		return;
	}

	UShort type = pack->packetType_;
	auto handler = handlerTable_.at(type);
	if (nullptr == handler)
	{
		// 패킷 삭제
		// 세션 연결 끊기

		delete pack;
		pack = nullptr;
		return;
	}

	handler(pack->dataBuffer_, pack->dataBytes_);

	delete pack;
	pack = nullptr;
}