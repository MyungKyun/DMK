#pragma once
#include "Type.h"
#include "DBQuery.h"

struct TestQuery : public DBQuery
{
	Short     curId_;
	Short	  newId_;

	TestQuery(Short curId, Short newId)
		: DBQuery(L"sp_UpdateTestVal(?,?)")
		, curId_(curId)
		, newId_(newId)
	{
	}
};

struct TestChangeAccountIdQuery : public DBQuery
{
	WString oName_;
	WString nName_;

	TestChangeAccountIdQuery(const WString& on, const WString& nn)
		: DBQuery(L"sp_UpdateAccountId(?,?)")
		, oName_(on)
		, nName_(nn)
	{
	}
};

struct TestSetUserAccountInfo : public DBQuery
{
	WString name_;
	WString pass_;

	TestSetUserAccountInfo(const WString& name, const WString& pss)
		: DBQuery(L"SetAccount(?,?)")
		, name_(name)
		, pass_(pss)
	{
	}

	
};

struct TestGetQuery : public DBQuery
{
	UInt64	uid;
	
	WChar outName_[30];
	WChar outPass_[30];
	TestGetQuery(UInt64 id)
		: DBQuery(L"GetAccount(?)")
		, uid(id)
		, outName_(L"")
	{
	}

	virtual Bool PreparingParams() final
	{
		auto ret = BindParam(SQL_PARAM_INPUT, uid);
		if (ret != SQL_SUCCESS) { return false; }

		return true;
	}

	Bool GetColumnData() override
	{
		GetColData(outName_, _countof(outName_));
		GetColData(outPass_, _countof(outPass_));

		LOG_WARN(L"UserId: {}, Pass: {}", outName_, outPass_);

		return true;
	}
};

struct TestOutputParamQuery : public DBQuery
{
	Int id_;
	Int outval1_;
	WChar	outval2_[30] = { 0, };

	TestOutputParamQuery(Int id)
		: DBQuery(L"spGetTestValue2(?)")
		, id_(id)
	{
	}

	virtual Bool PreparingParams() final
	{
		auto ret = BindParam(SQL_PARAM_INPUT, id_);
		if (ret != SQL_SUCCESS)	{ return false; }

	/*	ret= BindParam(SQL_PARAM_OUTPUT, outval1_);
		if (ret != SQL_SUCCESS) { return false; }

		ret = BindParam(SQL_PARAM_OUTPUT, outval2_, _countof(outval2_));
		if (ret != SQL_SUCCESS) { return false; }*/

		return true;
	}

	Bool GetColumnData() override
	{
		GetColData(outval1_);
		GetColData(outval2_, _countof(outval2_));

		return true;
	}

	Void Print()
	{
		LOG_WARN(L"first Value: {}, second: {}", outval1_, outval2_);
	}

};
