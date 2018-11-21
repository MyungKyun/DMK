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
	
	using SessionQueue = concurrency::concurrent_queue<std::shared_ptr<Session>>;
	
	SessionQueue					sessionQue_;
	Short							poolCount_;

public:
	SessionPool();
	~SessionPool();

	Bool							MakeSessionPool(NetworkDepartment* networkDept, UShort poolCount);
	
	SessionSptr						GetSession();
	
	Void							SessionReturns(const std::shared_ptr<Session>& sessionPtr);
		
};