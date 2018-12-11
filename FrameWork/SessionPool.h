#pragma once


using SessionSptr = std::shared_ptr<Session>;


class BaseSessionPool
{
public:

	virtual SessionSptr GetSession() = 0;
	virtual Void		SessionReturns(std::shared_ptr<Session>& sessionPtr) = 0;
};


template <typename SessionType>
class SessionPool : public BaseSessionPool
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

	SessionPool(UShort poolCount, Int bufferSize = 128 * 1024)
		: poolCount_(poolCount)
	{
		if (false == makeSession(poolCount, bufferSize))
		{
			assert(false);
		}			
	}

	~SessionPool()
	{

	}

	SessionSptr		GetSession() override
	{
		std::shared_ptr<Session> sessionPtr;
		if (sessionQue_.try_pop(sessionPtr))
		{
			return  sessionPtr;
		}

		return nullptr;
	}

	Void		SessionReturns(std::shared_ptr<Session>& sessionPtr) override
	{
		sessionQue_.push(sessionPtr);
	}

private:

	Bool	makeSession(UShort poolCount, Int bufferSize = 128 * 1024)
	{
		poolCount_ = poolCount;
		if (poolCount_ <= 0)
		{
			return false;
		}

		for (auto i = 0; i < poolCount; ++i)
		{
			auto sessionPtr = std::make_shared<SessionType>(bufferSize);
			if (nullptr == sessionPtr)
			{
				LOG_ERROR(L"sessionPtr is null");
				return false;
			}

			sessionQue_.push(sessionPtr);

		}

		return true;
	}
		
};
