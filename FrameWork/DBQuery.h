#pragma once


class DBQuery
{
	WString			queryStatement_;
	Int				paramIdx_;
	Int				columnIdx_;
protected:

	SQLHSTMT		hstmt_;
	virtual			Bool	GetColumnData() { return true; }

public:

	DBQuery();
	explicit DBQuery(const WString& queryStatement);
	virtual ~DBQuery();

	Bool		Execute( DBConnection* const connection);

	virtual Bool		PreparingParams(DBConnection* const connection);
	
	SQLRETURN BindParam(SQLSMALLINT ioType, Char& val);
	SQLRETURN BindParam(SQLSMALLINT ioType, Byte& val);
	SQLRETURN BindParam(SQLSMALLINT ioType, Short& val);
	SQLRETURN BindParam(SQLSMALLINT ioType, UShort& val);
	SQLRETURN BindParam(SQLSMALLINT ioType, Int& val);
	SQLRETURN BindParam(SQLSMALLINT ioType, UInt& val);
	SQLRETURN BindParam(SQLSMALLINT ioType, Int64& val);
	SQLRETURN BindParam(SQLSMALLINT ioType, UInt64& val);
	
	SQLRETURN BindParam(SQLSMALLINT ioType, WChar* val, Int len = 0);

	///////////////////////////////////////////////////////////////////

	
	SQLRETURN GetColData(Char& out);
	SQLRETURN GetColData(Byte& out);
	SQLRETURN GetColData(Short& out);
	SQLRETURN GetColData(UShort& out);
	SQLRETURN GetColData(Int& out);
	SQLRETURN GetColData(UInt& out);
	SQLRETURN GetColData(Int64& out);
	SQLRETURN GetColData(UInt64& out);

	SQLRETURN GetColData(WChar* out, Int strSize);
	
};
