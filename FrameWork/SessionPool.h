#pragma once

class ServerIocp;

using SessionSptr = std::shared_ptr<Session>;

class SessionPool
{

	enum
	{
		// юс╫ц
		IO_BUFFER_SIZE = 128 * 1024,
	};
	
	using SessionQueue = std::deque< std::shared_ptr<Session>>;
	using AliveSessionMap = std::unordered_map< UDLong, std::shared_ptr<Session>>;

	SessionQueue					sessionQue_;
	AliveSessionMap					sessionMap_;
	Short							poolCount_;
	Lock							mutex_;
	UDLong							latestSessionId_;

public:
	SessionPool();
	~SessionPool();

	Bool							MakeSessionPool(ServerNetWorkDepartment* serverNetWorkDept, UShort poolCount, ReceiveProcessor* recvProcessor);
	
	SessionSptr						GetSession();
	
	std::shared_ptr<Session>&		Find(UDLong sessionId);
	Void							ReturnSession(UDLong sessionId);
		
};