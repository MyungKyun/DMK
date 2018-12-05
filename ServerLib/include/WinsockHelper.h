#pragma once

namespace WinsockHelper
{
	SOCKET		CreateTcpSocket();
	Void		ErrorDisplay(const WString& msg);
	Void        NagleOff(SOCKET socket);

}
