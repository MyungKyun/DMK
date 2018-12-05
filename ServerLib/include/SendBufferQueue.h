#pragma once

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

	Void SetData(Byte* buffer)
	{
		::memcpy_s(buffer_, len_, buffer, len_);
	}
	
	~SendBuffer()
	{
		delete[] buffer_;
		buffer_ = nullptr;
		len_ = 0;
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
	
	Void		Push(std::shared_ptr<SendBuffer>&& sendBuffer, Int len, Bool& sendImmediately);
	Void		Copy(Byte* buf, Int buffersize, Int& outSize, Int& numberOfSend);
	Bool		Empty();
	Bool		NeedMoreSend(Int numberOfSent, Int numberOfTransferredBytes);

private:

	Bool		pop(Byte* buf, Int bufferSize, Int& outSize, Int& numberOfSend);
	Bool		copyFromReserve(Byte* buf, Int bufferSize, Int& outSize, Int& numberOfSend);
};