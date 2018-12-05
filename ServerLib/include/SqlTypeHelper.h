#pragma once
#include "stdafx.h"
#include <sql.h>
#include <sqlext.h>

namespace SqlTypeHelper
{
	using TypeInfo = std::pair<SQLSMALLINT, SQLSMALLINT>;

	template <typename T>
	std::enable_if_t<(std::is_reference_v<T>) && (std::is_same_v<Short, std::remove_reference_t<T>>), TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_SMALLINT), SQLSMALLINT(SQL_C_SSHORT));
	}

	template <typename T>
	std::enable_if_t<(std::is_reference_v<T>) && (std::is_same_v<UShort, std::remove_reference_t<T>>), TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_SMALLINT), SQLSMALLINT(SQL_C_USHORT));
	}

	template <typename T>
	std::enable_if_t<(std::is_reference_v<T>) && (std::is_same_v<Int64, std::remove_reference_t<T>>), TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_BIGINT), SQLSMALLINT(SQL_C_SBIGINT));
	}

	template <typename T>
	std::enable_if_t<(std::is_reference_v<T>) && (std::is_same_v<UInt64, std::remove_reference_t<T>>), TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_BIGINT), SQLSMALLINT(SQL_C_UBIGINT));
	}
		
	template<typename T>
	std::enable_if_t<(std::is_reference_v<T>) && (std::is_same_v<Int, std::remove_reference_t<T>>), TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_INTEGER), SQLSMALLINT(SQL_C_SLONG));
	}

	template <typename T>
	std::enable_if_t<(std::is_reference_v<T>) && (std::is_same_v<UInt, std::remove_reference_t<T>>), TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_SMALLINT), SQLSMALLINT(SQL_C_ULONG));
	}

	template <typename T>
	std::enable_if_t<(std::is_reference_v<T>) && (std::is_same_v<Char, std::remove_reference_t<T>>), TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_TINYINT), SQLSMALLINT(SQL_C_TINYINT));
	}

	template <typename T>
	std::enable_if_t<(std::is_reference_v<T>) && (std::is_same_v<Byte, std::remove_reference_t<T>>), TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_TINYINT), SQLSMALLINT(SQL_C_UTINYINT));
	}

	
	template <typename T>
	std::enable_if_t< (std::is_pointer_v<T>) && (std::is_same_v<WChar, std::remove_const_t<std::remove_pointer_t<T>>>), TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_WVARCHAR), SQLSMALLINT(SQL_C_WCHAR));
	}

	template <typename T>
	std::enable_if_t < (std::is_array_v<T>) && (std::is_same_v<WChar, std::remove_all_extents_t<T>>), TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_WVARCHAR), SQLSMALLINT(SQL_C_WCHAR));
	}


	template <typename T>
	std::enable_if_t<(std::is_reference_v<T> && std::is_array_v<T>) && (std::is_same_v<WChar, std::remove_all_extents_t<std::remove_reference_t<T>>>), TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_WVARCHAR), SQLSMALLINT(SQL_C_WCHAR));
	}

	template <typename T>
	std::enable_if_t<(std::is_pointer_v<T> && std::is_array_v<T>) && (std::is_same_v<WChar, std::remove_all_extents_t<T>>), TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_WVARCHAR), SQLSMALLINT(SQL_C_WCHAR));
	}


	template <typename T>
	std::enable_if_t<std::is_enum_v<T>, TypeInfo> GetType()
	{
		return GetType<std::underlying_type_t<T>>();
	}
	

	template <typename T>
	std::enable_if_t<std::is_same_v<Char,T>, TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_TINYINT), SQLSMALLINT(SQL_C_TINYINT));
	}

	template <typename T>
	std::enable_if_t<std::is_same_v<Byte,T>, TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_TINYINT), SQLSMALLINT(SQL_C_UTINYINT));
	}

	template <typename T>
	std::enable_if_t<std::is_same_v<Int, T>, TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_INTEGER), SQLSMALLINT(SQL_C_SLONG));
	}

	template <typename T>
	std::enable_if_t<std::is_same_v<Short, T>, TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_SMALLINT), SQLSMALLINT(SQL_C_SSHORT));
	}

	template <typename T>
	std::enable_if_t<std::is_same_v<UShort, T>, TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_SMALLINT), SQLSMALLINT(SQL_C_USHORT));
	}
	
	template <typename T>
	std::enable_if_t<std::is_same_v<UInt, T>, TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_SMALLINT), SQLSMALLINT(SQL_C_ULONG));
	}

	template <typename T>
	std::enable_if_t<std::is_same_v<Long, T>, TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_INTEGER), SQLSMALLINT(SQL_C_SLONG));
	}

	template <typename T>
	std::enable_if_t<std::is_same_v<ULong, T>, TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_INTEGER), SQLSMALLINT(SQL_C_ULONG));
	}

	template <typename T>
	std::enable_if_t<std::is_same_v<Int64, T>, TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_BIGINT), SQLSMALLINT(SQL_C_SBIGINT));
	}

	template <typename T>
	std::enable_if_t<std::is_same_v<UInt64, T>, TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_BIGINT), SQLSMALLINT(SQL_C_UBIGINT));
	}

	template <typename T>
	std::enable_if_t<std::is_same_v<Float, T>, TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_REAL), SQLSMALLINT(SQL_C_FLOAT));
	}

	template <typename T>
	std::enable_if_t<std::is_same_v<Double, T>, TypeInfo> GetType()
	{
		return TypeInfo::pair(SQLSMALLINT(SQL_DOUBLE), SQLSMALLINT(SQL_C_DOUBLE));
	}

}//namespace