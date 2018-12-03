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

	
	SQLRETURN GetColData(SQLSMALLINT cType, Char& out);
	SQLRETURN GetColData(SQLSMALLINT cType, Byte& out);
	SQLRETURN GetColData(SQLSMALLINT cType, Short& out);
	SQLRETURN GetColData(SQLSMALLINT cType, UShort& out);
	SQLRETURN GetColData(SQLSMALLINT cType, Int& out);
	SQLRETURN GetColData(SQLSMALLINT cType, UInt& out);
	SQLRETURN GetColData(SQLSMALLINT cType, Int64& out);
	SQLRETURN GetColData(SQLSMALLINT cType, UInt64& out);

	SQLRETURN GetColData(SQLSMALLINT cType, WChar* out, Int strSize);
	
};
