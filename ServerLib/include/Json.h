#pragma once
#include "rapidjson/document.h"


namespace Parser
{
	class Json
	{

	public:

		using Document = rapidjson::Document;
		using Value = rapidjson::Value&;

	private:

		Document   document_;
		std::FILE* fp_;

	public:
		Json() = default;
		~Json() {};

		Bool Read(const WString& file);
		Document&	GetDocument();
	};

}
