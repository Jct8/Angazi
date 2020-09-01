#include "Precompiled.h"
#include "UdpSocket.h"

#include "SocketAddress.h"

using namespace Angazi::Network;

bool UdpSocket::Open()
{
	ASSERT(mSocket == INVALID_SOCKET, "Network - UDP Socket already opened!");
	mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mSocket == INVALID_SOCKET)
	{
		LOG("Failed to open socket.");
		return false;
	}
	return true;
}

void UdpSocket::Close()
{
	if (mSocket != INVALID_SOCKET)
	{
		closesocket(mSocket);
		mSocket = INVALID_SOCKET;
	}
}

bool UdpSocket::SetNonBlocking(bool nonBlocking)
{
	ASSERT(mSocket != INVALID_SOCKET, "Network - UDP Socket not open!");
	u_long arg = nonBlocking ? 1 : 0;
	int result = ioctlsocket(mSocket, FIONBIO, &arg);
	if (result == SOCKET_ERROR)
	{
		LOG("Network - Failed to set non blocking mode in TCP socket. Error: %d", WSAGetLastError());
		Close();
		return false;
	}
	return true;
}

bool UdpSocket::SetToBroadcast(bool broadcast)
{
	ASSERT(mSocket != INVALID_SOCKET, "Network - UDP Socket not open!");
	int result = setsockopt(mSocket, SOL_SOCKET, broadcast ? SO_BROADCAST: SO_REUSEADDR, (char*)&mOptVal, sizeof(mOptVal));
	if (result == SOCKET_ERROR)
	{
		LOG("Network - UDP Socket failed to set socket option to broadcast. Error: %d", WSAGetLastError());
		return false;
	}
	return true;
}

bool UdpSocket::Bind(const SocketAddress & address)
{
	ASSERT(mSocket != INVALID_SOCKET, "Network - UDP Socket not open!");
	int result = bind(mSocket, &address.mSockAddr, static_cast<int>(address.GetSize()));
	if (result == SOCKET_ERROR)
	{
		LOG("Network - UDP Socket failed to bind. Error: %d",WSAGetLastError());
		return false;
	}
	return true;
}

int UdpSocket::SendTo(const SocketAddress & toAddress, const uint8_t * buffer, int length)
{
	int bytesSent = sendto(mSocket, reinterpret_cast<const char*>(buffer), length, 0, &toAddress.mSockAddr, static_cast<int>(toAddress.GetSize()));
	if (bytesSent < 0)
		LOG("Network - UDP Socket failed to send. Error type:%d", WSAGetLastError());
	return bytesSent;
}

int UdpSocket::ReceiveFrom(const SocketAddress & fromAddress, uint8_t * buffer, int length)
{
	int addrLen = static_cast<int>(fromAddress.GetSize());
	int bytesReceived = recvfrom(mSocket, reinterpret_cast<char*>(buffer), length, 0, (sockaddr*)&fromAddress.mSockAddr, &addrLen);

	if (bytesReceived > 0)
		return bytesReceived;

	int error = WSAGetLastError();
	if (error == WSAEWOULDBLOCK)
		return 0;

	if (error == WSAECONNRESET)
	{
		LOG(" Network - UdpSocket Connection reset.");
		Close();
		return -WSAECONNRESET;
	}

	LOG("Network - UdpSocket Error on receive.");
	Close();
	return -error;
}
