#include "stdafx.h"

ReceiveProcessor::ReceiveProcessor()
{

}

ReceiveProcessor::~ReceiveProcessor()
{

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
			WinsockHelper::ErrorDisplay(L"preparingReceive() WSARecv failed.");//�ӽ����

			//������� �ʿ�
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
	
	auto& sessionPtr = overlappedPreRecv->sessionSPtr;

	if (false == sessionPtr->IsConnected())
	{
		return;
	}

	auto  recvBuf = sessionPtr->GetRecvBuf();
	auto& recvBegin = sessionPtr->GetRecvBegin();
	auto& recvEnd = sessionPtr->GetRecvEnd();

	Int bufferSize = recvEnd - recvBegin;
	if (bufferSize == 0)
	{
		sessionPtr->ResetBufferBeginSize();
		sessionPtr->ResetBufferEndSize();
	}
	else if (bufferSize < 0)
	{
		// ��Ŀ��Ʈ ���Ѿ��Ѵ�.
		sessionPtr->ResetBufferBeginSize();
		sessionPtr->ResetBufferEndSize();
	}
	else
	{
		
		::memmove(recvBuf, recvBuf + recvBegin, bufferSize);
		recvBegin = 0;
		recvEnd = bufferSize;
	}

	SOCKET socket = sessionPtr->GetSocket();

	Int totalBufferSize = sessionPtr->GetBufferSize();

	Overlapped_Ex* overlappedRecv = new Overlapped_Ex_Processing_Receive(this, socket, recvBuf + recvEnd, totalBufferSize - recvEnd, sessionPtr);
	
	DWORD recvBytes = 0;
	DWORD flags = 0;

	if (SOCKET_ERROR == ::WSARecv(socket, &(overlappedRecv->wsabuf_), 1, &recvBytes, &flags, overlappedRecv, nullptr))
	{
		
		WinsockHelper::ErrorDisplay(L"ReceiveProcessor::postReceive failed.");//�ӽ����
		//������� �ʿ�
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

	auto  recvBuffer = overlappedRecv->sessionSPtr->GetRecvBuf();
	auto& recvBegin = overlappedRecv->sessionSPtr->GetRecvBegin();
	auto& recvEnd = overlappedRecv->sessionSPtr->GetRecvEnd();
	Int totalBufferSize = overlappedRecv->sessionSPtr->GetBufferSize();

	recvEnd += numberOfTransferredBytes;
	
	if (recvEnd > totalBufferSize)
	{
		return;
	}

	Int bufferLength = recvEnd - recvBegin;
	Int processedLen = 0;
	Int processStandByBytes = bufferLength;

	while (processStandByBytes >= sizeof(PacketHeader))
	{
		const PacketHeader* header = reinterpret_cast<const PacketHeader*>(recvBuffer);

		if (header->size < sizeof(PacketHeader) || totalBufferSize < header->size)
		{
			return;
		}

		if (processStandByBytes < header->size)
		{
			break;
		}

		
		auto realDatabuffer = recvBuffer + sizeof(PacketHeader);
		
		
		DataPack* dPack = new DataPack(100, realDatabuffer, header->size - sizeof(PacketHeader), overlappedRecv->sessionSPtr );

		// ��Ŷ ť���� ���� ����, Io Thread�� ��Ŷ���� ó���ϰ� ����ó�� ������� �ѱ�� ����� �����غ���.
		//contentsLogicProcess_->Push(dPack);

		if (GisEmptyPacketQueue_.load() == true)
		{
			GisEmptyPacketQueue_.store(false);
			GhasPacketDataCond_.notify_all();
		}

		recvBuffer += header->size;
		processStandByBytes -= header->size;
	}


	recvBegin += (bufferLength - processStandByBytes);

	ReservingReceive(overlappedRecv->sessionSPtr);


	delete overlappedRecv;
	overlappedRecv = nullptr;
}
