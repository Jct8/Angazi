#include "Precompiled.h"
#include "SocketAddress.h"

using namespace Angazi;
using namespace Angazi::Network;

SocketAddress::SocketAddress()
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetIP4() = INADDR_ANY;
	GetPort() = 0;
}

SocketAddress::SocketAddress(const char * host, uint16_t port)
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetPort() = htons(port);
	inet_pton(AF_INET, host, &GetIP4());
}

SocketAddress::SocketAddress(uint16_t port)
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetIP4() = INADDR_ANY;
	GetPort() = htons(port);
}

//char * SocketAddress::GetIPAddress() const
//{
//	char ipstr[INET_ADDRSTRLEN];
//	inet_ntop(AF_INET, &GetAsSockAddrIn()->sin_addr, ipstr, sizeof(ipstr));
//	return ipstr;
//}