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