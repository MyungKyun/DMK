#pragma once
#include "Type.h"
#include "DBQuery.h"

struct TestQuery : public DBQuery
{
	Short curId_;
	Short newId_;

	TestQuery(Short curId, Short newId)
		: DBQuery(L"{call sp_UpdateTestVal(?,?)}")
		, curId_(curId)
		, newId_(newId)
	{
		BindParams(&curId_, &newId_);
	}
};

struct TestStringQuery : public DBQuery
{
	WString oName_;
	WString nName_;

	TestStringQuery(const WString& on, const WString& nn)
		: DBQuery(L"{call sp_UpdateAccountId(?,?)}")
		, oName_(on)
		, nName_(nn)
	{
		BindParams(oName_.c_str(), nName_.c_str());
	}
};
