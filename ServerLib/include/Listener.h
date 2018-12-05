#pragma once

class Listener
{

	SOCKET		listenSocket_;
	

public:
	Listener();
	~Listener();


	Bool	Listen(const IPv4& address);

	SOCKET	GetListenSocket();
};
