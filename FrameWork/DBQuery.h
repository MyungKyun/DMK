#pragma once

struct ParamData
{
	SQLUSMALLINT	paramOrder_;			//  (?,?,?) param 순서 (1) 부터시작
	SQLSMALLINT		ioType_;				//	input, output
	SQLSMALLINT		cType_;					//	C 언어에서 사용하는 데이터 타입
	SQLSMALLINT		sType_;					//	SQL 데이터 타입
	SQLUINTEGER		columnSize_;			//  파라미터의 데이터 사이즈 지정
	SQLSMALLINT		decimalDigits_;			//  바이너리 데이터에서는 사용 않음,
	SQLPOINTER		paramValuePtr_;			//  데이터의 포인터
	SQLLEN			paramValueSize_;		//  데이터의 크기
	SQLLEN			strLen_or_lnd_;			//  SQL_DATA_AT_EXEC 입력, SQLPutData() 함수를 통해서 데이터를 보낸다고 알려줌.
											//  문자열사용시 SQL_NTS

	ParamData(SQLSMALLINT sqlType, SQLSMALLINT cType, SQLSMALLINT ioType_)
		: paramOrder_(0)
		, ioType_(ioType_)
		, cType_(cType)
		, sType_(sqlType)
		, columnSize_(0)
		, decimalDigits_(0)
		, paramValuePtr_(nullptr)
		, paramValueSize_(0)
		, strLen_or_lnd_(0)
	{
	}
};


class DBQuery
{

	using ParamDatas = std::vector<ParamData>;
	
	WString			queryStatement_;
	ParamDatas		params_;

protected:

	SQLHSTMT		hstmt_;
	virtual			Bool	GetColumnData() { return true; }

public:

	DBQuery();
	explicit DBQuery(const WString& queryStatement);
	virtual ~DBQuery();

	template <typename Arg, typename... Args>
	Void BindParams(Arg&& arg, Args&&... args)
	{
		bind(std::forward<Arg>(arg));
		BindParams(std::forward<Args>(args)...);
	}

	Void BindParams()
	{
	}

	template<typename Arg, typename... Args>
	Void BindOutParams(Arg&& arg, Args&&... args)
	{
		bindOut(std::forward<Arg>(arg));
		BindOutParams(std::forward<Args>(args)...);
	}

	Void BindOutParams()
	{
	}

	Bool		Execute( DBConnection* const connection);

private:

	template<typename T>
	Void bindOut(T&& value)
	{
		auto[sqltype, ctype] = SqlTypeHelper::GetType<T>();
		ParamData param(sqltype, ctype, SQL_PARAM_OUTPUT);
		param.paramValuePtr_ = &value;
		param.paramValueSize_ = sizeof(T);
	}

	template<>
	Void bindOut(wchar_t*&& val)
	{
		ParamData param(SQL_WVARCHAR, SQL_C_WCHAR, SQL_PARAM_INPUT);
		param.paramValuePtr_ = const_cast<wchar_t*>(val);
		param.paramValueSize_ = 0;
		param.strLen_or_lnd_ = SQL_NTSL;
	}

	template<typename T>
	Void bind(T&& value)
	{
		auto[sqltype, ctype] = SqlTypeHelper::GetType<T>();
		ParamData param(sqltype, ctype, SQL_PARAM_INPUT);
		param.paramValuePtr_ = &value;
		param.paramValueSize_ = sizeof(T);

		params_.emplace_back(param);
	}

	template<>
	void bind(const wchar_t*&& val)
	{
		ParamData param(SQL_WVARCHAR, SQL_C_WCHAR, SQL_PARAM_INPUT);
		param.paramValuePtr_ = const_cast<wchar_t*>(val);
		param.paramValueSize_ = (::wcslen(val) + 1) * sizeof(wchar_t);
		param.strLen_or_lnd_ = SQL_NTSL;

		params_.emplace_back(param);
	}


	Bool		preparingParams(DBConnection* const connection);
};
