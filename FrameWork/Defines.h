#pragma once


enum
{
	PACKET_MAX_SIZE = 65535,
};


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


enum
{
	MILLI_PER_SEC = 1000,
	NANO_PER_SEC = 1000000000,
	NANO_PER_MILLI = 1000000
};



// chrono type
using time_point_t = std::chrono::time_point<std::chrono::system_clock>;
using second_t = std::chrono::seconds;
using milli_t = std::chrono::milliseconds;
using time_point_t = std::chrono::time_point<std::chrono::system_clock>;




class Uncopyable
{
private:

	Uncopyable(const Uncopyable&);
	Uncopyable(const Uncopyable&&);
	Uncopyable& operator=(const Uncopyable&);
	Uncopyable&& operator=(const Uncopyable&&);

protected:

	Uncopyable() {}
	virtual ~Uncopyable() {}
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


template <typename ObjectType, typename... FnArgs, typename... TupArgs>
decltype(auto) ExecuteMemFunc(std::shared_ptr<ObjectType> object, Void(ObjectType::*memFunc)(FnArgs...), const std::tuple<TupArgs...>& targs)
{
	return ExecuteMemFunc(object, memFunc, targs, std::make_index_sequence <sizeof...(TupArgs)>());
}

template <typename ObjectType, typename... FnArgs, typename... TupArgs, std::size_t... idx>
decltype(auto) ExecuteMemFunc(std::shared_ptr<ObjectType> object, Void(ObjectType::*memFunc)(FnArgs...), const std::tuple<TupArgs...>& targs, std::index_sequence<idx...>)
{
	return (object.get()->*memFunc)(std::get<idx>(targs)...);
}