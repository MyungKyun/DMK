// TestServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


class TestServer : public Server
{

	SessionPool*			sessionPool_;

public:
	TestServer(Dispatcher* packetDispatcher)
		: Server(packetDispatcher)
	{

	}

	Bool	Setup()
	{
		sessionPool_ = new SessionPool();
		
		if (false == Server::Setup(sessionPool_))
		{
			return false;
		}

		GThreadManager.AddDepartment(NetworkIoThread::GetDepartmentNumber(), &iocp_);
		GThreadManager.Start<NetworkIoThread>(0, "NetworkIoThread", &iocp_);


		return true;
	}

	~TestServer() 
	{
		delete sessionPool_;
	}
};

//Bool  handler_default(std::shared_ptr<Session> session, const PacketHeader* header, Byte* buf, Int transferredBytes);

class TestPacketDispathcer : public Dispatcher
{
public:
	
	TestPacketDispathcer()
	{
		InitHandlers(&handler_default);

		Register(10, &handler_Msg_Process);
	}
	~TestPacketDispathcer() {}

private:

	static Bool  handler_default(std::shared_ptr<Session> session, const PacketHeader* header, Byte* buf, Int transferredBytes)
	{
		return true;
	}
	
	static Bool	handler_Msg_Process(std::shared_ptr<Session> session, const PacketHeader* header, Byte* buf, Int transferredBytes)
	{
		MessagePacket* msg = reinterpret_cast< MessagePacket*>(buf);

		//std::cout << msg->val << std::endl;
		//overlappedRecv->sessionSPtr_->Send(recvBuf_, header->size);
		
		Int bufSize = sizeof(PacketHeader) + transferredBytes;
		

		//SendBuffer buffer(bufSize);
//		PacketHeader* header = reinterpret_cast<PacketHeader*>();

		//buffer.SetData(buf);
		//session->Send(buffer, bufSize);
		return true;
	}
};





int main()
{

	std::shared_ptr<TestServer> server = std::make_shared<TestServer>(new TestPacketDispathcer);
	
	if (false == server->Setup())
	{
		return -1;
	}

	server->Run();

	Sleep(INFINITE);

    return 0;
}

