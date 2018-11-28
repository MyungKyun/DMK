#include "stdafx.h"

ReceiveProcessor::ReceiveProcessor()
	: recvBegin_(0)
	, recvEnd_(0)
	, totalRecvBufferSize_(IO_BUFFER_SIZE)
{
	recvBuf_ = new Byte[IO_BUFFER_SIZE];
}

ReceiveProcessor::~ReceiveProcessor()
{
	delete[] recvBuf_;
	recvBuf_ = nullptr;
}


Void	ReceiveProcessor::CompleteIoEventProcess(Overlapped_Ex* overlapped, Int numberOfTransferredBytes, Bool ioEventResult)
{
	switch (overlapped->ioType_)
	{
	case IO_RESERVING_RECV:
		preparingReceive(overlapped, numberOfTransferredBytes);
		break;

	case IO_RECV:
		processingReceive(overlapped, numberOfTransferredBytes);
		break;
	}

	delete overlapped;
	overlapped = nullptr;
}

Bool	ReceiveProcessor::ReservingReceive(std::shared_ptr<Session> sessionPtr)
{
	SOCKET socket = sessionPtr->GetSocket();

	Overlapped_Ex* overlappedPreRecv = new Overlapped_Ex_Preparing_Receive(this, sessionPtr);

	DWORD recvBytes = 0;
	DWORD flags = 0;

	if (SOCKET_ERROR == ::WSARecv(socket, &(overlappedPreRecv->wsabuf_), 1, &recvBytes, &flags, overlappedPreRecv, nullptr))
	{
		auto error = ::WSAGetLastError();

		if (error != WSA_IO_PENDING)
		{
			LOG_ERROR(L"ReservingReceive Failed Error:{0}", error);

			//연결끊기 필요
			delete overlappedPreRecv;
			overlappedPreRecv = nullptr;

			return false;
		}
	}

	return true;
}

Void	ReceiveProcessor::preparingReceive(Overlapped_Ex* overlapped, Int numberOfTransferredBytes)
{
	postReceive(overlapped, numberOfTransferredBytes);
}

Void	ReceiveProcessor::postReceive(Overlapped_Ex* overlapped, Int numberOfTransferredBytes)
{
	Overlapped_Ex_Preparing_Receive* overlappedPreRecv = static_cast<Overlapped_Ex_Preparing_Receive*>(overlapped);
	if (nullptr == overlappedPreRecv)
	{
		assert(false);
	}
	
	auto& sessionPtr = overlappedPreRecv->sessionSPtr_;

	if (false == sessionPtr->IsConnected())
	{
		return;
	}


	Int bufferSize = recvEnd_ - recvBegin_;
	if (bufferSize == 0)
	{
		recvEnd_ = 0;
		recvBegin_ = 0;
	}
	else if (bufferSize < 0)
	{
		// 디스커넥트 시켜야한다.
		recvEnd_ = 0;
		recvBegin_ = 0;
	}
	else
	{
		
		::memmove(recvBuf_, recvBuf_ + recvBegin_, bufferSize);
		recvBegin_ = 0;
		recvEnd_ = bufferSize;
	}

	SOCKET socket = sessionPtr->GetSocket();

	Overlapped_Ex* overlappedRecv = new Overlapped_Ex_Processing_Receive(this, socket, recvBuf_ + recvEnd_, totalRecvBufferSize_ - recvEnd_, sessionPtr);
	
	DWORD recvBytes = 0;
	DWORD flags = 0;

	if (SOCKET_ERROR == ::WSARecv(socket, &(overlappedRecv->wsabuf_), 1, &recvBytes, &flags, overlappedRecv, nullptr))
	{
		auto error = WSAGetLastError();
		LOG_ERROR(L"postReceive Failed. Error:{0}", error);

		//연결끊기 필요
		delete overlappedRecv;
		overlappedRecv = nullptr;
		return;
	}
}


Void	ReceiveProcessor::processingReceive(Overlapped_Ex* overlapped, Int numberOfTransferredBytes)
{	
	Overlapped_Ex_Processing_Receive* overlappedRecv = static_cast<Overlapped_Ex_Processing_Receive*>(overlapped);

	recvEnd_ += numberOfTransferredBytes;
	
	if (recvEnd_ > totalRecvBufferSize_)
	{
		return;
	}

	Int bufferLength = recvEnd_ - recvBegin_;
	Int processedLen = 0;
	Int processStandByBytes = bufferLength;

	const Byte* buf = recvBuf_;

	while (processStandByBytes >= sizeof(PacketHeader))
	{
		const PacketHeader* header = reinterpret_cast<const PacketHeader*>(buf);

		if (header->size < sizeof(PacketHeader) || totalRecvBufferSize_ < header->size)
		{
			return;
		}

		if (processStandByBytes < header->size)
		{
			break;
		}

		overlappedRecv->sessionSPtr_->GetNetworkDept()->Dispatch(overlappedRecv->sessionSPtr_, header, buf + sizeof(PacketHeader), header->size - sizeof(PacketHeader));
	
		buf += header->size;
		processStandByBytes -= header->size;
	}

	processedLen = bufferLength - processStandByBytes;

	if (bufferLength < processedLen)
	{
		//망
		//disconnect
	}
	else
	{
		recvBegin_ += processedLen;
		ReservingReceive(overlappedRecv->sessionSPtr_);
	}
}
