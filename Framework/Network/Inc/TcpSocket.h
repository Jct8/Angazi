#pragma once

namespace Angazi::Network
{
	class SocketAddress;

	class TcpSocket
	{
	public:
		bool Open();
		void Close();
		bool SetNoDelay(bool noDelay);
		bool SetNonBlocking(bool nonBlocking);

		bool Connect(const SocketAddress& socketAddress);
		bool Bind(const SocketAddress& socketAddress);
		bool Listen(int backLog = 10);
		std::unique_ptr<TcpSocket> Accept(SocketAddress& socketAddress);

		int Send(const uint8_t* buffer, int length);
		int Receive(uint8_t* buffer, int length);

		const SOCKET& GetSocket() { return mSocket; };
	private:
		SOCKET mSocket = INVALID_SOCKET;
	};
}