#include "stdafx.h"

SendBufferQueue::SendBufferQueue()
{
	std::atomic_init(&currentCount_, 0);
	std::atomic_init(&remainingTransferBytes_, 0);
}

SendBufferQueue::~SendBufferQueue()
{
	/*std::shared_ptr<SendBuffer> sendBuffer;
	while (sendQue_.try_pop(sendBuffer))
	{
		sendBuffer.reset();
	}*/

	SendBuffer* sendBuffer = nullptr;
	while (sendQue_.try_pop(sendBuffer))
	{
		delete sendBuffer;
	}
}

Bool	SendBufferQueue::Empty()
{
	return sendQue_.empty();
}

Void	SendBufferQueue::Push(Byte* buf, Int len, Bool& sendImmediately)
{
	currentCount_.fetch_add(1);

	SendBuffer* sendBuffer = new SendBuffer(len);
	sendBuffer->SetData(buf);

	remainingTransferBytes_.fetch_add(len);

	sendQue_.push(sendBuffer);

	if (1== currentCount_.load())
	{
		sendImmediately = true;
	}
}

Bool	SendBufferQueue::NeedMoreSend(Int numberOfSent, Int numberOfTransferredBytes)
{
	remainingTransferBytes_.fetch_sub(numberOfTransferredBytes);
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
	Int remainBufSize = bufferSize;
	Bool needMoreCopy = true;

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
			reservingTo_.erase(reservingTo_.begin(), first);
			needMoreCopy = false;
			break;
		}

		::memcpy_s(buf, remainBufSize, bufPtr->buffer_, bufPtr->len_);

		outSize += bufPtr->len_;
		remainBufSize -= bufPtr->len_;
		++numberOfSend;
		delete bufPtr;
	}
	
	if (true == needMoreCopy)
	{
		pop(buf + outSize, remainBufSize, outSize, numberOfSend);
	}
	else
	{
		reservingTo_.clear();
	}
	
}


Void	SendBufferQueue::pop(Byte* buf,Int bufferSize, Int& outSize, Int& numberOfSend)
{
	Int remainSize = bufferSize;
	Bool isEnough = true;
	
	while (false == sendQue_.empty())
	{
		SendBuffer* sendBuffer = nullptr;
		sendQue_.try_pop(sendBuffer);

		if (nullptr != sendBuffer)
		{
			if (0 >= remainSize ||
				remainSize < sendBuffer->len_)
			{
				isEnough = false;
			}

			if (isEnough)
			{
				::memcpy_s(buf, remainSize, sendBuffer->buffer_, sendBuffer->len_);

				outSize += sendBuffer->len_;
				remainSize -= sendBuffer->len_;
				++numberOfSend;
				
				delete sendBuffer;
				sendBuffer = nullptr;
			}
			else
			{
				reservingTo_.push_back(sendBuffer);
				continue;
			}
		}
	}
}