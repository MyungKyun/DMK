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

Void	SendBufferQueue::Push(std::shared_ptr<SendBuffer> sendBuffer, Bool& sendImmediately)
{
	sendQue_.push(sendBuffer);
	currentCount_.fetch_add(1);

	if (1 == currentCount_)
	{
		sendImmediately = true;
	}
}


Void	SendBufferQueue::Pop(std::shared_ptr<SendBuffer> out)
{
	if (true == sendQue_.try_pop(out))
	{
		currentCount_.fetch_sub(1);
	}
}