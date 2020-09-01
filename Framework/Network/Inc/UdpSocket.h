#pragma once

namespace Angazi::Network
{
	class SocketAddress;

	class UdpSocket
	{
	public:
		UdpSocket() = default;
		~UdpSocket() {}

		bool Open();
		void Close();

		bool SetNonBlocking(bool nonBlocking);
		bool SetToBroadcast(bool broadcast);

		bool Bind(const SocketAddress& address);

		int SendTo(const SocketAddress& toAddress, const uint8_t* buffer, int length);
		int ReceiveFrom(const SocketAddress& fromAddress, uint8_t* buffer, int length);

	private:
		UdpSocket(const UdpSocket&) = delete;
		UdpSocket& operator=(const UdpSocket&) = delete;

		SOCKET mSocket = INVALID_SOCKET;
		int mOptVal = 1;
	};
}