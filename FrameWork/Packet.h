#pragma once

#pragma pack(push,1)
struct PacketHeader
{
	UShort		size;
	UShort		type;
};
#pragma pack(pop)



// 테스트용 패킷
struct MessagePacket
{
	//std::string msg;
	Int val;
};
/////////////////

struct DataPack
{
	UShort	packetType_ = 0;
	Byte*	dataBuffer_;
	Int     dataBytes_ = 0;

	DataPack(UShort type, const Byte* buf, Int dataBytes)
		: packetType_(type)
		, dataBytes_(dataBytes)
	{
		dataBuffer_ = new Byte[dataBytes];
		::memcpy_s(dataBuffer_, dataBytes, buf, dataBytes);
	}

	~DataPack()
	{
		delete dataBuffer_;
		dataBuffer_ = { 0, };
		dataBytes_ = 0;
		packetType_ = 0;
	}

private:

	DataPack(const DataPack&);
	DataPack(const DataPack&&);
	DataPack& operator=(const DataPack&);
	DataPack&& operator=(const DataPack&&);

};
