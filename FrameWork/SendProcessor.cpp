#include "stdafx.h"


SendProcessor::SendProcessor()
{

}

SendProcessor::~SendProcessor()
{

}


Void	SendProcessor::Send()
{
	// 버퍼에 저장해둔다.
	// 보내기전에 버퍼에서 보낼 데이터를 꺼내온다.
	//	  예약된 것 먼저 꺼내온다.
	// 보낼수 있을때 정해둔 크기만큼 보낸다.
	// 여유가 되면 바로 보낸다.
}

Void SendProcessor::CompleteIoEventProcess(Overlapped_Ex * overlapped, Int numberOfTransferredBytes, Bool ioEventResult)
{
	
}
