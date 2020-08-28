#include "Precompiled.h"
#include "TcpSocket.h"

#include "SocketAddress.h"

using namespace Angazi;
using namespace Angazi::Network;

bool TcpSocket::Open()
{
	ASSERT(mSocket == INVALID_SOCKET, "Network - Socket already opened!");
	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSocket == INVALID_SOCKET)
	{
		LOG("Network - Failed to open socket.");
		return false;
	}
	return true;
}

void TcpSocket::Close()
{
	if (mSocket != INVALID_SOCKET)
	{
		closesocket(mSocket);
		mSocket = INVALID_SOCKET;
	}
}

bool TcpSocket::SetNoDelay(bool noDelay)
{
	ASSERT(mSocket != INVALID_SOCKET, "Network - Socket not opened!");
	int delayArg = noDelay ? 1 : 0;
	int result = setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char *)&delayArg, sizeof(int));
	if (result == SOCKET_ERROR)
	{
		LOG("Network - Failed to set TcpSocket no delay. Error: %d", WSAGetLastError());
		Close();
		return false;
	}
	return true;
}

bool TcpSocket::SetNonBlocking(bool nonBlocking)
{
	ASSERT(mSocket != INVALID_SOCKET, "Network - Socket not opened!");
	u_long arg = nonBlocking ? 1 : 0;
	int result = ioctlsocket(mSocket, FIONBIO, &arg);
	if (result == SOCKET_ERROR)
	{
		LOG("Network - Failed to set TcpSocket non blocking mode. Error: %d", WSAGetLastError());
		Close();
		return false;
	}
	return true;
}

bool TcpSocket::Connect(const SocketAddress & socketAddress)
{
	ASSERT(mSocket != INVALID_SOCKET, "Network - Socket not opened!");
	int hr = connect(mSocket, &socketAddress.mSockAddr, static_cast<int>(socketAddress.GetSize()));
	if (hr == SOCKET_ERROR)
	{
		int lastError = WSAGetLastError();
		if (lastError == WSAEWOULDBLOCK)
		{
			fd_set write, error;
			FD_ZERO(&write);
			FD_ZERO(&error);
			FD_SET(mSocket, &write);
			FD_SET(mSocket, &error);

			TIMEVAL timeout = { 10, 0 }; // 10 second timeout
			int result = select(0, NULL, &write, &error, &timeout);
			if (result == 0)
			{
				LOG("Network - TcpSocket connect timed out.");
				return false;
			}
			else
			{
				if (FD_ISSET(mSocket, &write))
					return true;
				if (FD_ISSET(mSocket, &error))
				{
					LOG("Network - Select Error.");
					return false;
				}
			}
		}
		else
		{
			LOG("Network - Failed to connect Tcp socket. Error %d", lastError);
			return false;
		}
	}
	return true;
}

bool TcpSocket::Bind(const SocketAddress & socketAddress)
{
	int result = bind(mSocket, (LPSOCKADDR)&socketAddress.mSockAddr, static_cast<int>(socketAddress.GetSize()));
	if (result == SOCKET_ERROR)
	{
		LOG("Network - TcpSocket failed to bind socket. Error %d", WSAGetLastError());
		Close();
		return false;
	}
	return true;
}

bool TcpSocket::Listen(int backLog)
{
	int result = listen(mSocket, backLog);
	if (result == SOCKET_ERROR)
	{
		LOG("Network - TCPSocket failed to listen socket. Error: %d", WSAGetLastError());
		Close();
		return false;
	}
	return true;
}

std::unique_ptr<TcpSocket> TcpSocket::Accept(SocketAddress & socketAddress)
{
	int addrLength = static_cast<int>(socketAddress.GetSize());
	SOCKET incomingSocket = accept(mSocket, &socketAddress.mSockAddr, &addrLength);
	if (incomingSocket == INVALID_SOCKET)
	{
		int lastError = WSAGetLastError();
		if (lastError != WSAEWOULDBLOCK)
		{
			LOG("Network - TcpSocket failed to accept connection. Error: %d", lastError);
			Close();
		}
		return nullptr;
	}
	std::unique_ptr<TcpSocket> returnSocket = std::make_unique<TcpSocket>();
	returnSocket->mSocket = incomingSocket;
	return returnSocket;
}

int TcpSocket::Send(const uint8_t * buffer, int length)
{
	int bytesSent = send(mSocket, reinterpret_cast<const char*>(buffer), length, 0);
	if (bytesSent < 0)
	{
		LOG("Network - TcpSocket Failed to send. Error: %d", WSAGetLastError());
		Close();
	}
	return bytesSent;
}

int TcpSocket::Receive(uint8_t * buffer, int length)
{
	int bytesReceived = recv(mSocket, reinterpret_cast<char*>(buffer), length, 0);
	if (bytesReceived == SOCKET_ERROR)
	{
		int lastError = WSAGetLastError();
		if (lastError == WSAEWOULDBLOCK)
		{
			bytesReceived = 0;
		}
		/*else if (lastError == WSAECONNABORTED || lastError == WSAECONNRESET)
		{
			bytesReceived = -1;
		}*/
		else
		{
			LOG("Network - TcpSocket Failed to receive data. Error: %d", WSAGetLastError());
			Close();
			return SOCKET_ERROR;
		}
	}
	return bytesReceived;
}
