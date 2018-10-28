#pragma once

enum
{
	PACKET_MAX_SIZE = 65535,
};

struct SendBuffer
{
	Byte*		buffer_;
	Int			len_;

	SendBuffer(Int packetSize)
		: len_(packetSize)
	{
		assert(packetSize <= PACKET_MAX_SIZE);
		buffer_ = new Byte[packetSize];
	}

	~SendBuffer()
	{

	}

	Void	SetData(Byte* buffer)
	{
		::memcpy_s(buffer_, len_, buffer, len_);
	}
};


class SendBufferQueue
{
	concurrency::concurrent_queue<std::shared_ptr<SendBuffer>>	sendQue_;
	std::atomic_int												currentCount_;

public:

	SendBufferQueue();
	~SendBufferQueue();
	
	Void		Push(std::shared_ptr<SendBuffer> sendBuffer, Bool& sendImmediately);
	Void		Pop(std::shared_ptr<SendBuffer> out);
};