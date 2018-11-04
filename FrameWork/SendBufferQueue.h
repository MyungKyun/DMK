#pragma once

enum
{
	PACKET_MAX_SIZE = 65535,
};

struct SendBuffer
{
	Byte*		buffer_;
	Int			len_;

	explicit SendBuffer(Int packetSize = PACKET_MAX_SIZE)
		: len_(packetSize)
	{
		assert(packetSize <= PACKET_MAX_SIZE);
		buffer_ = new Byte[packetSize];
	}

	~SendBuffer()
	{
	}

};


class SendBufferQueue
{
	concurrency::concurrent_queue<std::shared_ptr<SendBuffer>>	sendQue_;
	std::atomic_int												currentCount_;
	std::vector<std::shared_ptr<SendBuffer>>					reservingTo_;

public:

	SendBufferQueue();
	~SendBufferQueue();
	
	Void		Push(Byte* buf, Int len, Bool& sendImmediately);
	Void		Copy(Byte* buf, Int buffersize, Int& outSize);
	Bool		Empty();
private:

	Void		pop(Byte* buf, Int bufferSize, Int& outSize);
};