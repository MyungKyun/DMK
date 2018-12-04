#pragma once

class IPv4
{
	sockaddr_in		sockAddr_;

public:

	IPv4();
	IPv4(const sockaddr* sa, Int len);
	IPv4(const Char* ip, UShort port);
	IPv4(const WChar* ip, UShort port);



	Int						GetFamily()			const;
	const	sockaddr*		GetSockAddr()		const;
	Int						GetSockAddrLen()	const;

	WString					GetIpString()		const;
	UShort					GetPort()			const;
	String					GetIpChar()			const;
private:

	Void	setAddress(const Char* ip, UShort port);
};