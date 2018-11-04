#pragma once


enum
{
	NETWORK_IO_PROCESSING_DEPT = 0,
	PACKET_PROCESSING_DEPT = 1,

	MAX_DEPT,
};

enum
{
	// юс╫ц
	IO_BUFFER_SIZE = 128 * 1024,
};


class IdGenerator : public Singleton<IdGenerator>
{
	std::atomic<UDLong>		sessionIdGen_{ 0 };

public:

	UDLong				SessionIdGenerate()
	{
		return sessionIdGen_.fetch_add(1);
	}
};

#define		GIDGen		Singleton<IdGenerator>::GetInstance()