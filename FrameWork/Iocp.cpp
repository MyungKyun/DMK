#include "stdafx.h"

Iocp::Iocp(DWORD numberOfConcurrentThreads)
	: isRunning_(true)
{
	iocp_ = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, numberOfConcurrentThreads);
}

Iocp::~Iocp()
{
	ClosePort();
}

Void Iocp::Resister(HANDLE handle, ULONG_PTR completionkey)
{
	if (false == isRunning_)
	{
		return;
	}

	::CreateIoCompletionPort(handle, iocp_, completionkey, 0);
}

Void Iocp::ListenToCompletionPort()
{
	while (isRunning_)
	{
		DWORD transferredBytes = 0;
		ULONG_PTR completionKey = 0;
		OVERLAPPED* overlapped = nullptr;
		Overlapped_Ex* overlappedEx = nullptr;

		//타임아웃은 우선 무한으로.
		Bool result = ::GetQueuedCompletionStatus(GetHandle(), &transferredBytes, &completionKey, &overlapped, INFINITE);
		overlappedEx = static_cast<Overlapped_Ex*>(overlapped);

		if (false == result)
		{
			auto error = ::WSAGetLastError();
			if (nullptr == overlapped)
			{
				if (WAIT_TIMEOUT == error)
				{
					return;
				}
				else if (ERROR_ABANDONED_WAIT_0)
				{
					return;
				}

				return;
			}
			else
			{
				if (INVALID_SOCKET != overlappedEx->socket_)
				{
					DWORD flags = 0;
					if (false == ::WSAGetOverlappedResult(overlappedEx->socket_, overlappedEx, &transferredBytes, false, &flags))
					{
						auto error = ::WSAGetLastError();
					}
					else
					{
						result = true;
					}
					
				}
			}
		}

		if (nullptr != overlappedEx &&
			nullptr != overlappedEx->processor_)
		{
			overlappedEx->processor_->CompleteIoEventProcess(overlappedEx, transferredBytes, result != false);
		}
		else
		{
			delete overlappedEx;
			overlappedEx = nullptr;
		}
	}
}

Void Iocp::ClosePort()
{
	if (iocp_ != NULL)
	{
		::CloseHandle(iocp_);
		iocp_ = NULL;
	}
}

Void Iocp::Stop()
{
	isRunning_ = false;
}


HANDLE Iocp::GetHandle() const
{
	return iocp_;
}