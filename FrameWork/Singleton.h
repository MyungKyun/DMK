#pragma once

#include "stdafx.h"

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


template <typename T>
class Singleton : private Uncopyable
{

	static std::once_flag		onceFlag_;
	static std::unique_ptr<T>	sInstance;

public:

	template<typename... Args>
	static T& GetInstance(Args&&... args)
	{
		std::call_once(onceFlag_, [](Args&&... args) ->  Void { sInstance.reset(new T(std::forward<Args>(args)...)); },
			std::forward<Args>(args)...);

		return *sInstance;
	}
};


template<typename T>
std::unique_ptr<T> Singleton<T>::sInstance;

template<typename T>
std::once_flag Singleton<T>::onceFlag_;



