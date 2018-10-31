#pragma once

class NetworkDepartmentManager
{

	using Departments = std::vector<NetworkDepartment*>;
	Departments			depts_;

public:

	NetworkDepartmentManager();
	~NetworkDepartmentManager();

	template <typename DeptType, typename... Args>
	Bool MakeDeaprtment(Args&&... args)
	{
		static_assert(std::is_base_of<NetworkDepartment, DeptType>::value, "Failed. NetworkDepartment Type miss matched.");

		decltype(auto) deptPtr = new DeptType(std::forward<Args>(args)...);
		
		if (false == deptPtr->Setup())
		{
			return false;
		}

		depts_.push_back(deptPtr);
		
		return true;
	}
};
