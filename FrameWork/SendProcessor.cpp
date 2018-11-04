#include "stdafx.h"


SendProcessor::SendProcessor()
{
	sendBuf_ = new Byte[IO_BUFFER_SIZE];
}

SendProcessor::~SendProcessor()
{
	delete[] sendBuf_;
	sendBuf_ = nullptr;
}


Void	SendProcessor::PostSend(std::shared_ptr<Session>& sessionPtr, Byte* buf, Int len)
{
	Bool sendImmediately = false;
	
	sendBufferQue_.Push(buf, len, sendImmediately);

	if (sendImmediately)
	{
		postSend(sessionPtr);
	}
}

Void SendProcessor::CompleteIoEventProcess(Overlapped_Ex * overlapped, Int numberOfTransferredBytes, Bool ioEventResult)
{
	Overlapped_Ex_Send* overlappedSend = static_cast<Overlapped_Ex_Send*>(overlapped);

	if (numberOfTransferredBytes != overlappedSend->wsabuf_.len)
	{
		return;
	}

	if (false == sendBufferQue_.Empty())
	{
		postSend(overlappedSend->sessionSPtr_);
	}

	delete overlappedSend;
	overlappedSend = nullptr;
}


Void SendProcessor::postSend(std::shared_ptr<Session>& sessionPtr)
{
	Int sendSize = 0;
	
	sendBufferQue_.Copy(sendBuf_, IO_BUFFER_SIZE, sendSize);

	Overlapped_Ex* overlappedSend = new Overlapped_Ex_Send(this, sessionPtr->GetSocket(), sendBuf_, sendSize, sessionPtr);

	DWORD sendBytes = 0;
	if (SOCKET_ERROR == ::WSASend(sessionPtr->GetSocket(), &(overlappedSend->wsabuf_), 1, &sendBytes, 0, overlappedSend, nullptr))
	{
		auto error = ::WSAGetLastError();
		if (WSA_IO_PENDING != error)
		{

			WinsockHelper::ErrorDisplay(L"postSend() failed.");
			//disconnect

			delete overlappedSend;
			overlappedSend = nullptr;
			return;
		}
	}
}
