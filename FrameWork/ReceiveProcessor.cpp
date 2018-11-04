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

Void	ReceiveProcessor::Setup(PacketProcess* contentLogicProcess)
{
	if (nullptr == contentLogicProcess)
	{
		assert(false);
	}

	contentsLogicProcess_ = contentLogicProcess;
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
}

Bool	ReceiveProcessor::ReservingReceive(const std::shared_ptr<Session>& sessionPtr)
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
			WinsockHelper::ErrorDisplay(L"preparingReceive() WSARecv failed.");//임시출력

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
		
		WinsockHelper::ErrorDisplay(L"ReceiveProcessor::postReceive failed.");//임시출력
		//연결끊기 필요
		delete overlappedRecv;
		overlappedRecv = nullptr;
		return;
	}

	delete overlappedPreRecv;
	overlappedPreRecv = nullptr;
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

	while (processStandByBytes >= sizeof(PacketHeader))
	{
		const PacketHeader* header = reinterpret_cast<const PacketHeader*>(recvBuf_);

		if (header->size < sizeof(PacketHeader) || totalRecvBufferSize_ < header->size)
		{
			return;
		}

		if (processStandByBytes < header->size)
		{
			break;
		}

		
		auto realDatabuffer = recvBuf_ + sizeof(PacketHeader);
		
		
		//DataPack* dPack = new DataPack(100, realDatabuffer, header->size - sizeof(PacketHeader), overlappedRecv->sessionSPtr );

		MessagePacket* msg = reinterpret_cast<MessagePacket*>(realDatabuffer);
		printf("[%d]\n", msg->val);

		//Echo Test
		overlappedRecv->sessionSPtr_->Send(recvBuf_, header->size);



		// 패킷 큐잉을 하지 말고, Io Thread가 패킷까지 처리하고 로직처리 스레드로 넘기는 방안을 생각해보자.
		//contentsLogicProcess_->Push(dPack);

		/*if (GisEmptyPacketQueue_.load() == true)
		{
			GisEmptyPacketQueue_.store(false);
			GhasPacketDataCond_.notify_all();
		}*/

		recvBuf_ += header->size;
		processStandByBytes -= header->size;
	}


	recvBegin_ += (bufferLength - processStandByBytes);

	ReservingReceive(overlappedRecv->sessionSPtr_);


	delete overlappedRecv;
	overlappedRecv = nullptr;
}
