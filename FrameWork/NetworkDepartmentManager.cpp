#include "stdafx.h"

NetworkDepartmentManager::NetworkDepartmentManager()
{

}

NetworkDepartmentManager::~NetworkDepartmentManager()
{
	for (auto& dept : depts_)
	{
		delete dept;
		dept = nullptr;
	}

	depts_.clear();
}