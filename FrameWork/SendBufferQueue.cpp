#include "stdafx.h"

SendBufferQueue::SendBufferQueue()
{
	std::atomic_init(&currentCount_, 0);
}

SendBufferQueue::~SendBufferQueue()
{

	std::shared_ptr<SendBuffer> sendBuffer;
	while (sendQue_.try_pop(sendBuffer))
	{
		sendBuffer.reset();
	}
}

Bool	SendBufferQueue::Empty()
{
	return sendQue_.empty();
}

Void	SendBufferQueue::Push(std::shared_ptr<SendBuffer>&& sendBuffer, Int len, Bool& sendImmediately)
{
	currentCount_.fetch_add(1);

	sendQue_.push(std::move(sendBuffer));

	if (1== currentCount_.load())
	{
		sendImmediately = true;
	}
}

Bool	SendBufferQueue::NeedMoreSend(Int numberOfSent, Int numberOfTransferredBytes)
{
	currentCount_.fetch_sub(numberOfSent);
	if (0 >= currentCount_.load())
	{
		return false;
	}

	return true;
}

Void	SendBufferQueue::Copy(Byte* buf, Int bufferSize, Int& outSize, Int& numberOfSend)
{
	outSize = 0;
	numberOfSend = 0;
	
	if (0 >= bufferSize)
	{
		assert(false);
	}

	if (false == copyFromReserve(buf, bufferSize, outSize, numberOfSend))
	{
		return;
	}

	pop(buf, bufferSize, outSize, numberOfSend);
	
}

Bool	SendBufferQueue::copyFromReserve(Byte* buf, Int bufferSize, Int& outSize, Int& numberOfSend)
{
	auto first = reservingTo_.begin();
	auto last = reservingTo_.end();

	for (; first != last; ++first)
	{
		auto bufPtr = *first;

		if (bufferSize - outSize < bufPtr->len_)
		{
			reservingTo_.erase(reservingTo_.begin(), first);
			return false;
		}

		::memcpy_s(buf, bufPtr->len_, bufPtr->buffer_, bufPtr->len_);

		outSize += bufPtr->len_;
		++numberOfSend;
	}

	reservingTo_.clear();
	return true;
}

Bool	SendBufferQueue::pop(Byte* buf,Int bufferSize, Int& outSize, Int& numberOfSend)
{
	Bool isEnough = true;

	std::shared_ptr<SendBuffer> sendBuffer = nullptr;
	while (sendQue_.try_pop(sendBuffer))
	{
		if (nullptr == sendBuffer)
		{
			continue;
		}

		if (0 >= bufferSize ||
			bufferSize - outSize < sendBuffer->len_)
		{
			isEnough = false;
		}

		if (isEnough)
		{
			::memcpy_s(buf, sendBuffer->len_, sendBuffer->buffer_, sendBuffer->len_);

			outSize += sendBuffer->len_;
			++numberOfSend;
		}
		else
		{
			reservingTo_.push_back(sendBuffer);
		}
	}

	return true;
}