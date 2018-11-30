#pragma once

class Job : private Noncopyable
{
public:
	
	virtual ~Job() {}
	virtual Void Execute() = 0;
};


template <typename ObjectType, typename... Args>
class MemFuncJob : public Job
{
	using MemFunc = Void(ObjectType::*)(Args...);

	std::shared_ptr<ObjectType>		object_;
	std::tuple<Args...>				args_;
	MemFunc							memFn_;

public:

	explicit MemFuncJob(std::shared_ptr<ObjectType> object, MemFunc memFn, Args... args)
		: object_(object)
		, memFn_(memFn)
		, args_(std::forward<Args>(args)...)
	{
	}

	~MemFuncJob()
	{
	}

	Void Execute() override
	{
		ExecuteMemFunc(object_, memFn_, args_);
	}
};