#pragma once

class NetworkIoThread : public Thread
{
	Iocp*	iocp_;

public:

	static Short	GetDepartmentNumber()
	{
		return NETWORK_IO_PROCESSING_DEPT;
	}

	NetworkIoThread(DWORD id, Iocp* iocp, const String& name)
		: Thread(id, name, NETWORK_IO_PROCESSING_DEPT)
		, iocp_(iocp ? iocp : nullptr)
	{

	}

	Bool		Setup() final
	{
		if (false == __super::Setup())
		{
			return false;
		}

		return true;
	}

	Void		Run() final
	{
		while (false == isStop_)
		{
			iocp_->ListenToCompletionPort();
		}
	}

	Void		ShutDown() final
	{

	}
};
