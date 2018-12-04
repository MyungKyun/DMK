#include "stdafx.h"

IPv4::IPv4()
{
	::memset(&sockAddr_, 0, sizeof(sockAddr_));

	sockAddr_.sin_family = AF_INET;
	sockAddr_.sin_addr.s_addr = ::htonl(INADDR_ANY);
	sockAddr_.sin_port = ::htons(0);
}

IPv4::IPv4(const sockaddr* sa, Int len)
{
	::memset(&sockAddr_, 0, sizeof(sockAddr_));

	if (sa != nullptr &&
		sa->sa_family == AF_INET &&
		len == sizeof(sockAddr_))
	{
		sockAddr_ = *(reinterpret_cast<const sockaddr_in*>(sa));
	}
}

IPv4::IPv4(const Char* ip, UShort port)
{
	setAddress(ip, port);
}

IPv4::IPv4(const WChar* ip, UShort port)
{
	Char aip[INET_ADDRSTRLEN] = { 0, };
	ConvertWideToChar(ip, aip, _countof(aip));
	setAddress(aip, port);
}


Int	IPv4::GetFamily()	const
{
	return sockAddr_.sin_family;
}

const sockaddr*	IPv4::GetSockAddr()	const
{
	return reinterpret_cast<const sockaddr*>(&sockAddr_);
}

Int	IPv4::GetSockAddrLen()	const
{
	return sizeof(sockAddr_);
}

String	IPv4::GetIpChar()	const
{
	Char aip[INET_ADDRSTRLEN] = { 0, };
	::inet_ntop(GetFamily(), const_cast<in_addr*>(&sockAddr_.sin_addr), aip, sizeof(aip));

	return aip;
}

WString IPv4::GetIpString()	const
{
	Char aip[INET_ADDRSTRLEN] = { 0, };
	::inet_ntop(GetFamily(), const_cast<in_addr*>(&sockAddr_.sin_addr), aip, sizeof(aip));

	WChar wip[INET_ADDRSTRLEN] = { 0, };
	ConvertCharToWide(aip, wip, _countof(wip));
	return wip;
}

UShort	IPv4::GetPort()		const
{
	return ::ntohs(sockAddr_.sin_port);
}

Void	IPv4::setAddress(const Char* ip, UShort port)
{
	::memset(&sockAddr_, 0, sizeof(sockAddr_));

	ULong ipv4Address = INADDR_NONE;
	if (::inet_pton(AF_INET, ip, &ipv4Address) != 0)
	{
		sockAddr_.sin_family = AF_INET;
		sockAddr_.sin_addr.s_addr = ipv4Address;
		sockAddr_.sin_port = ::htons(port);
	}
	else
	{
		addrinfo hints;
		::memset(&hints, 0, sizeof(hints));
		hints.ai_flags = AI_PASSIVE;
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		Char portNum[10] = { 0, };
		::_itoa_s(port, portNum, sizeof(portNum), 10);

		addrinfo* result = nullptr;
		if (::getaddrinfo(ip, portNum, &hints, &result) == 0)
		{
			for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next)
			{
				if (ptr->ai_family == AF_INET)
				{
					sockAddr_ = *(reinterpret_cast<const sockaddr_in*>(ptr->ai_addr));
					break;
				}
			}

			::freeaddrinfo(result);
		}
	}
}