#include "stdafx.h"

SendBufferQueue::SendBufferQueue()
	: currentCount_(0)
{

}

SendBufferQueue::~SendBufferQueue()
{
	std::shared_ptr<SendBuffer> sendBuffer;
	while (sendQue_.try_pop(sendBuffer))
	{
		sendBuffer.reset();
	}
}

Bool		SendBufferQueue::Empty()
{
	return sendQue_.empty();
}

Void	SendBufferQueue::Push(Byte* buf, Int len, Bool& sendImmediately)
{
	currentCount_.fetch_add(1);

	std::shared_ptr<SendBuffer> sendBuffer = std::make_shared<SendBuffer>();
	sendBuffer->buffer_ = buf;
	sendBuffer->len_ = len;

	sendQue_.push(sendBuffer);

	if (1 == currentCount_)
	{
		sendImmediately = true;
	}
}

Void		SendBufferQueue::Copy(Byte* buf, Int bufferSize, Int& outSize)
{
	outSize = 0;
	Int remainBufSize = bufferSize;
	if (0 >= remainBufSize)
	{
		assert(false);
	}

	auto first = reservingTo_.begin();
	auto last = reservingTo_.end();

	for (; first != last; ++first)
	{
		auto bufPtr = *first;
		
		if (remainBufSize < bufPtr->len_)
		{
			reservingTo_.erase(std::begin(reservingTo_), first);
			return;
		}

		::memcpy_s(buf, remainBufSize, bufPtr->buffer_, bufPtr->len_);

		outSize += bufPtr->len_;
		remainBufSize -= bufPtr->len_;
	}
	
	pop(buf + outSize, remainBufSize, outSize);
}


Void	SendBufferQueue::pop(Byte* buf,Int bufferSize, Int& outSize)
{
	Int remainSize = bufferSize;

	while (false == sendQue_.empty())
	{
		std::shared_ptr<SendBuffer> sendBuffer;
		if (true == sendQue_.try_pop(sendBuffer))
		{
			currentCount_.fetch_sub(1);
		}

		if (0 >= remainSize || 
			remainSize < sendBuffer->len_)
		{
			reservingTo_.push_back(std::move(sendBuffer));
		}
		else
		{
			::memcpy_s(buf, remainSize, sendBuffer->buffer_, sendBuffer->len_);

			outSize += sendBuffer->len_;
			remainSize -= outSize;

		}
	}
}