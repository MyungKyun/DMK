#include "stdafx.h"
#include "rapidjson/filereadstream.h"

// tencent rapidjson »ç¿ë 

namespace Parser
{
	using Document = rapidjson::Document;
	using rapidjson::FileReadStream;
	const int readBufferSize = USHRT_MAX;

	Bool Json::Read(const WString& file)
	{
		errno_t err = ::_wfopen_s(&fp_, file.c_str(), L"rb");
		if (nullptr == fp_)
		{
			LOG_ERROR(L"Could not Open File : {}", file.c_str());
			return false;
		}

		Char buf[readBufferSize] = { 0, };
		FileReadStream readStream(fp_, buf, readBufferSize);

		Bool ret = !document_.ParseStream(readStream).HasParseError();

		::fclose(fp_);

		return ret;
	}

	Document&	Json::GetDocument()
	{
		return document_;
	}


}// namespace
