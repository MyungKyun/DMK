#include "stdafx.h"

Server::Server(PacketProcess* contentLogicProcess)
	: contentLogicProcess_(contentLogicProcess)
	, startThreadCount_(0)
	
{
	
}

Server::~Server()
{
	ShutDown();


	for (int i = 0; i < threads_.size(); ++i)
	{
		delete threads_[i];
		threads_[i] = nullptr;
	}

	delete contentLogicProcess_;
	contentLogicProcess_ = nullptr;
}

Bool Server::Setup(SessionPool* sessionPool)
{
	if (false == startup())
	{
		return false;
	}

	serverNetDept_ = new ServerNetWorkDepartment(&iocp_, sessionPool, IPv4("127.0.0.1", 20000), Count::TOTAL_ACCEPT_COUNT); //임시 주소
	if (nullptr == serverNetDept_)
	{
		return false;
	}

	if (false == serverNetDept_->Initialize(contentLogicProcess_))
	{
		return false;
	}

	if (false == contentLogicProcess_->Setup())
	{
		return false;
	}

	return true;
}

Void Server::Run()
{
	SYSTEM_INFO sys;
	::GetSystemInfo(&sys);

	if (startThreadCount_ <= 0)
	{
		startThreadCount_ = sys.dwNumberOfProcessors;
	}

	if (startThreadCount_ > sys.dwNumberOfProcessors)
	{
		startThreadCount_ = sys.dwNumberOfProcessors;
	}

	for (DWORD i = 0; i < startThreadCount_; ++i)
	{
		threads_[i] = _CREATE_THREAD(Server, threadRun, NETWORK_PROCESSING_DEPT );
	}
}

Void Server::ShutDown()
{
	::WSACleanup();

}

Bool Server::startup()
{
	WSADATA wsa;
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		//스타트업 실패
		std::cout << "Failed. WSAStartup" << std::endl;//임시출력
		return false;
	}

	if (LOBYTE(wsa.wVersion) != 2 || HIBYTE(wsa.wVersion) != 2)
	{
		::WSACleanup();
		std::cout << "not found version(2.2) winsock.dll" << std::endl;//임시출력
		return false;
	}
	
	if (false == EXFunction::SetupExFunction())
	{
		return false;
	}

	return true;
}


Void Server::threadRun()
{
	iocp_.ListenToCompletionPort();
}