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

	using	valueType = T;
	using   pointerType = valueType*;
	using	atomicType = std::atomic<pointerType>;
	using	mtxType = std::mutex;


	static	atomicType		sInstance_;
	static  mtxType			mtx_;

public:

	static pointerType GetInstance()
	{
		pointerType	temp = sInstance_.load();

		if (nullptr == temp)
		{
			std::lock_guard<mtxType> lock(mtx_);

			temp = sInstance_.load();

			if (nullptr == temp)
			{
				temp = new valueType;
				sInstance_.store(temp);
			}
		}

		return sInstance_;
	}


};

template <typename T>
typename Singleton<T>::atomicType Singleton<T>::sInstance_ = nullptr;

template <typename T>
typename Singleton<T>::mtxType Singleton<T>::mtx_;