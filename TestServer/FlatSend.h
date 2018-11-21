#pragma once
#include "flatbuffers/flatbuffers.h"

class MksBuilder
{
	flatbuffers::FlatBufferBuilder builder_;

public:

	MksBuilder()
		: builder_(2048)
	{}

	flatbuffers::FlatBufferBuilder& Get() { return builder_; }
};


inline Void SendToSession(std::shared_ptr<Session>& sessionPtr, UShort type, MksBuilder* builder)
{
	Int size = sizeof(PacketHeader) + builder->Get().GetSize();

	auto sendBufferPtr = std::make_shared<SendBuffer>(size);

	PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBufferPtr->buffer_);
	header->type = type;
	header->size = size;

	::memcpy_s(header + 1, size - sizeof(PacketHeader), builder->Get().GetBufferPointer(), builder->Get().GetSize());

	sessionPtr->Send(sendBufferPtr, size);
}