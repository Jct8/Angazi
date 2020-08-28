#include "Precompiled.h"
#include "NetworkUtil.h"

using namespace Angazi;
using namespace Angazi::Network;

bool Angazi::Network::Initialize()
{
	// Initialize Winsock version 2.2
	WSAData wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
	{
		LOG("Network - Failed to initialize Winsock.");
		return false;
	}
	return true;
}

bool Angazi::Network::Terminate()
{
	// Shutdown WinSock
	int result = WSACleanup();
	return result == 0;
}
