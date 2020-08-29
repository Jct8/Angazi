#include "Angazi/Inc/Angazi.h"
#include <iostream>
using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

#define MAXRECVSIZE 1024

int main(int argc, char* argv[])
{
	Network::Initialize();
	const int max_client = 4;

	Network::TcpSocket listeningSocket;
	std::vector<std::unique_ptr<Network::TcpSocket>> clients;
	std::vector<bool> clientsReady;

	listeningSocket.Open();
	std::cout << "Server for network shooter opened\n";

	//binding:
	Network::SocketAddress serverInfo(8888);
	listeningSocket.Bind(serverInfo);

	//Listening to incoming connection:
	listeningSocket.Listen(10);
	fd_set readfds;

	while (true)
	{
		bool allClientsReady = false;
		std::cout << "Waiting for clients to connect:" << std::endl;

		// Accepting Clients
		while (true)
		{
			for (size_t i = 0; i < clientsReady.size(); i++)
			{
				if (clientsReady[i] == false || clientsReady.size() < 2)
				{
					allClientsReady = false;
					break;
				}
				allClientsReady = true;
			}

			if (allClientsReady)
				break;

			FD_ZERO(&readfds);
			FD_SET(listeningSocket.GetSocket(), &readfds);
			for (size_t i = 0; i < clients.size(); ++i)
			{
				if (clients[i] != nullptr)
				{
					SOCKET s = clients[i].get()->GetSocket();
					if (s > 0)
						FD_SET(s, &readfds);
				}
			}

			// wait for an activity on any of the client sockets. Timeout is set to null meaning wait indefinitely
			int activity = select(0, &readfds, NULL, NULL, NULL);
			if (activity == SOCKET_ERROR)
			{
				std::cout << "Select returned error " << WSAGetLastError();
				continue;
			}

			// Accept new Client
			if (clients.size() < max_client && FD_ISSET(listeningSocket.GetSocket(), &readfds))
			{
				std::unique_ptr<Network::TcpSocket> incoming;
				Network::SocketAddress clientAddr;

				incoming = listeningSocket.Accept(clientAddr);

				if (incoming == nullptr)
					continue;
				clients.emplace_back(std::move(incoming));
				clientsReady.emplace_back(false);

				int playernumber = (int)clients.size() - 1;
				// Send player number
				Network::MemoryStream outStream(MAXRECVSIZE);
				Network::StreamWriter sr(outStream);
				sr.Write(playernumber);
				clients.back().get()->Send(outStream.GetData(), outStream.GetSize());

				std::cout << "Player " << playernumber << " connected." << std::endl;
			}

			// check if there is any activity on any of the clients: Input from client
			for (size_t i = 0; i < clients.size(); ++i)
			{
				if (clients[i] == nullptr)
					continue;
				SOCKET s = clients[i].get()->GetSocket();

				if (FD_ISSET(s, &readfds))
				{
					uint8_t buffer[MAXRECVSIZE];
					memset(buffer, 0, sizeof(buffer));

					int byteReceived = clients[i].get()->Receive(buffer, static_cast<int>(std::size(buffer)));
					if (byteReceived > 0)
					{
						bool ready = false;
						// Grab data from packet
						Network::MemoryStream inStream(buffer, byteReceived);
						Network::StreamReader sr(inStream);

						sr.Read(ready);
						clientsReady[i] = ready;
					}
					else if (byteReceived == -1)
					{
						std::cout << "Player " << i << " disconnected.\n";
						closesocket(s);
						clients[i].reset();
					}
				}
			}
		}


		// Send Active Clients
		int totalPlayers = (int)clients.size();
		Network::MemoryStream outStream(MAXRECVSIZE);
		Network::StreamWriter sr(outStream);
		sr.Write(totalPlayers);

		for (size_t i = 0; i < clients.size(); i++)
			clients[i].get()->Send(outStream.GetData(), outStream.GetSize());

		std::cout << "All Client connected" << std::endl;
		std::cout << "Game Starting" << std::endl;

		while (true) // Main Game loop
		{
			// clear our readfds:
			FD_ZERO(&readfds);
			for (size_t i = 0; i < clients.size(); ++i)
			{
				if (clients[i] != nullptr)
				{
					SOCKET s = clients[i].get()->GetSocket();
					if (s > 0)
						FD_SET(s, &readfds);
				}
			}

			// wait for an activity on any of the client sockets. Timeout is set to null meaning wait indefinitely
			int activity = select(0, &readfds, NULL, NULL, NULL);
			if (activity == SOCKET_ERROR)
			{
				std::cout << "Select returned error " << WSAGetLastError();
				continue;
			}

			// check if there is any activity on any of the clients: Input from client
			for (size_t i = 0; i < clients.size(); ++i)
			{
				if (clients[i] == nullptr)
					continue;
				SOCKET s = clients[i].get()->GetSocket();

				if (FD_ISSET(s, &readfds))
				{
					uint8_t buffer[MAXRECVSIZE];
					int bytesReceived = clients[i].get()->Receive(buffer, static_cast<int>(std::size(buffer)));
					if (bytesReceived > 0)
					{
						// Grab data from packet
						float player_x, player_y, dir_X, dir_Y;
						int shot = 0, playerNumber = 0;

						Network::MemoryStream inStream(buffer,bytesReceived);
						Network::StreamReader sr(inStream);

						sr.Read(playerNumber);
						sr.Read(player_x);
						sr.Read(player_y);
						sr.Read(dir_X);
						sr.Read(dir_Y);
						sr.Read(shot);

						if (playerNumber == -1)
						{
							std::cout << "Player " << i << " disconnected.\n";
							closesocket(s);
							clients[i].reset();
							continue;
						}

						////////////// Send to other clients //////////////
						Network::MemoryStream outStream(MAXRECVSIZE);
						Network::StreamWriter sw(outStream);

						sw.Write(i);		// Player number
						sw.Write(player_x); // Player x
						sw.Write(player_y);	// Player y
						sw.Write(dir_X);	// Player dir x
						sw.Write(dir_Y);	// Player dir x
						sw.Write(shot);		// Player'shooting

						for (size_t j = 0; j < clients.size(); ++j)
						{
							if (clients[j] == nullptr)
								continue;
							if (i != j)
								clients[j].get()->Send(outStream.GetData(), outStream.GetSize());
						}
						//std::cout << "player pos = (" << player_x << "," << player_y << ")"<< ", heading = (" << dir_X << "," << dir_Y << ")" << std::endl;
					}
					else if (bytesReceived == -1)
					{
						std::cout << "Player " << i << " disconnected.\n";
						closesocket(s);
						clients[i].reset();
					}
				}
			}

			// Check if all disconnected
			bool disconnect = true;
			for (size_t i = 0; i < clients.size(); ++i)
			{
				if (clients[i] != nullptr)
				{
					disconnect = false;
					break;
				}
				disconnect = true;
			}
			if (disconnect)
				break;
		}

		for (auto& client : clients)
		{
			if(client != nullptr)
				client->Close();
		}
		clients.clear();
		clientsReady.clear();

		std::cout << "Game Ended" << std::endl << std::endl;
	}
	listeningSocket.Close();
	Network::Terminate();

	return 0;
}