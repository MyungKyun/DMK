#pragma once

struct ParamData
{
	SQLUSMALLINT	paramOrder_;			//  (?,?,?) param ���� (1) ���ͽ���
	SQLSMALLINT		ioType_;				//	input, output
	SQLSMALLINT		cType_;					//	C ���� ����ϴ� ������ Ÿ��
	SQLSMALLINT		sType_;					//	SQL ������ Ÿ��
	SQLUINTEGER		columnSize_;			//  �Ķ������ ������ ������ ����
	SQLSMALLINT		decimalDigits_;			//  ���̳ʸ� �����Ϳ����� ��� ����,
	SQLPOINTER		paramValuePtr_;			//  �������� ������
	SQLLEN			paramValueSize_;		//  �������� ũ��
	SQLLEN			strLen_or_lnd_;			//  SQL_DATA_AT_EXEC �Է�, SQLPutData() �Լ��� ���ؼ� �����͸� �����ٰ� �˷���.
											//  ���ڿ����� SQL_NTS

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
