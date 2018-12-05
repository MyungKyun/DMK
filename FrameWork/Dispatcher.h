#pragma once
class Session;

using PacketHandlerFunction = Bool(*)(std::shared_ptr<Session> , const PacketHeader* , const Byte* , Int );

class BaseDispatcher
{
public:
	virtual ~BaseDispatcher() {}
	virtual Bool Dispatch(std::shared_ptr<Session> session, const PacketHeader* header, const Byte* buf, Int transferredBytes) = 0;
};

class Dispatcher : public BaseDispatcher
{

protected:
	PacketHandlerFunction		handlers_[65536] = { nullptr, };

public:

	Dispatcher() {}
	~Dispatcher(){}

	explicit Dispatcher(PacketHandlerFunction defaultFn)
	{
		for (Int i = 0; i < _countof(handlers_); ++i)
		{
			handlers_[i] = defaultFn;
		}
	}

	Void InitHandlers(PacketHandlerFunction defaultFn)
	{
		for (Int i = 0; i < _countof(handlers_); ++i)
		{
			handlers_[i] = defaultFn;
		}
	}


	Bool Dispatch(std::shared_ptr<Session> session, const PacketHeader* header, const Byte* buf, Int transferredBytes) final
	{
		if (header->type >= _countof(handlers_) ||
			nullptr == handlers_[header->type])
		{
			return false;
		}

		if (false == (*handlers_[header->type])(session, header, buf, transferredBytes))
		{
			return false;
		}

		return true;
	}

	Void Register(UShort packetId, PacketHandlerFunction handlerFn)
	{
		handlers_[packetId] = handlerFn;
	}

};