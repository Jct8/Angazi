#include "GameState.h"
#include "ImGui/Inc/imgui.h"
#include "Player.h"

#define MAXRECVSIZE 1024
using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

std::array<Player, 4> players;
std::vector<Angazi::Math::LineSegment> walls;

int totalPlayers = 0;

namespace
{
	enum class NetworkState
	{
		Connect,
		GamePlay
	};

	NetworkState currentNetworkState = NetworkState::Connect;
}


void GameState::ProcessReceive()
{
	while (true)
	{
		if (!clientSocket)
			continue;
		if (stopThread == true)
			return;

		uint8_t buffer[1024];
		int byteReceived = clientSocket->Receive(buffer, static_cast<int>(std::size(buffer)));
		if (byteReceived > 0)
		{
			// Grab data from packet
			int otherPlayerNum = 0, shot = 0;
			float player_x, player_y, dir_X, dir_Y;

			Network::MemoryStream instream(buffer, byteReceived);
			Network::StreamReader sr(instream);

			sr.Read(otherPlayerNum);
			sr.Read(player_x);
			sr.Read(player_y);
			sr.Read(dir_X);
			sr.Read(dir_Y);
			sr.Read(shot);

			players[otherPlayerNum].SetPosition({ player_x ,player_y });
			players[otherPlayerNum].SetHeading({ dir_X ,dir_Y });
			if (shot == 1)
				players[otherPlayerNum].Shoot();
			LOG("player %d pos = (%.2f,%.2f), heading = (%.2f,%.2f)", otherPlayerNum, player_x, player_y, dir_X, dir_Y);
		}
	}
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	InputSystem::Get()->ShowSystemCursor(false);
	mCrossHairTexture = TextureManager::Get()->Load("Network/crosshair.png");
	background = TextureManager::Get()->Load("Network/bg.jpg");
	Network::Initialize();

	clientSocket = std::make_unique<Network::TcpSocket>();

	clientSocket->Open();
	players[0].Initialize(0, 350.f, "Network/shooter_red.png", "Network/bar_red.png");
	players[1].Initialize(1, 350.f, "Network/shooter_green.png", "Network/bar_green.png");
	players[2].Initialize(2, 350.f, "Network/shooter_orange.png", "Network/bar_orange.png");
	players[3].Initialize(3, 350.f, "Network/shooter_blue.png", "Network/bar_blue.png");

	auto screenWidth = GraphicsSystem::Get()->GetBackBufferWidth();
	auto screenHeight = GraphicsSystem::Get()->GetBackBufferHeight();
	Vector2 center = { screenWidth * 0.5f, screenHeight * 0.5f };
	// Center block
	walls.push_back(LineSegment({ center.x - 100.0f, center.y }, { center.x, center.y - 100.0f }));
	walls.push_back(LineSegment({ center.x + 100.0f, center.y }, { center.x, center.y - 100.0f }));
	walls.push_back(LineSegment({ center.x - 100.0f, center.y }, { center.x, center.y + 100.0f }));
	walls.push_back(LineSegment({ center.x + 100.0f, center.y }, { center.x, center.y + 100.0f }));

	// Diagonal Walls
	walls.push_back(LineSegment({ center.x - 400.0f, center.y - 100.0f }, { center.x - 200.0f, center.y - 250.0f }));
	walls.push_back(LineSegment({ center.x - 400.0f, center.y + 100.0f }, { center.x - 200.0f, center.y + 250.0f }));
	walls.push_back(LineSegment({ center.x + 400.0f, center.y - 100.0f }, { center.x + 200.0f, center.y - 250.0f }));
	walls.push_back(LineSegment({ center.x + 400.0f, center.y + 100.0f }, { center.x + 200.0f, center.y + 250.0f }));

	// Side Walls
	walls.push_back(LineSegment({ 0.0f , center.y }, { 200.0f, center.y }));
	walls.push_back(LineSegment({ screenWidth - 200.0f , center.y }, { screenWidth * 1.0f, center.y }));
	walls.push_back(LineSegment({ center.x , 0.0f }, { center.x , 150.0f }));
	walls.push_back(LineSegment({ center.x , screenHeight - 150.0f }, { center.x , screenHeight * 1.0f }));
}

void GameState::Terminate()
{
	if (receiveThread.joinable())
	{
		stopThread = true;
		receiveThread.join();
	}
	if (clientSocket)
		clientSocket->Close();
	clientSocket.reset();
	for (auto& player : players)
		player.Terminate();
	Network::Terminate();
}

void GameState::Update(float deltaTime)
{
	switch (currentNetworkState)
	{
	case NetworkState::Connect:
		ConnectToServer(deltaTime);
		break;
	case NetworkState::GamePlay:
		RunGamePlay(deltaTime);
		break;
	default:
		break;
	}
}

void GameState::RunGamePlay(float deltaTime)
{
	float dt = Math::Min(deltaTime, 1.0f / 60.0f);
	auto screenWidth = GraphicsSystem::Get()->GetBackBufferWidth();
	auto screenHeight = GraphicsSystem::Get()->GetBackBufferHeight();
	auto inputSystem = InputSystem::Get();

	Math::Vector2 bgPosition = { 0.0f, static_cast<float>(screenHeight) };
	BatchRenderer::Get()->AddSprite(background, bgPosition, Pivot::BottomLeft);

	Vector2 targetPosition = { static_cast<float>(inputSystem->GetMouseScreenX()) , static_cast<float>(inputSystem->GetMouseScreenY()) };
	BatchRenderer::Get()->AddSprite(mCrossHairTexture, targetPosition);

	for (auto& wall : walls)
		SimpleDraw::AddScreenLine(wall.from, wall.to, Colors::Cyan);

	for (auto& player : players)
	{
		if (player.IsActive())
			player.Update(dt);
	}

	// create state packet
	// Send if moved
	if (players[myPlayerNumber].GetMoveState())
	{
		float x, y, dirX, dirY;
		int shot = 0;

		Network::MemoryStream outstream(MAXRECVSIZE);
		Network::StreamWriter sw(outstream);

		x = players[myPlayerNumber].GetPosition().x;
		y = players[myPlayerNumber].GetPosition().y;
		dirX = players[myPlayerNumber].GetHeading().x;
		dirY = players[myPlayerNumber].GetHeading().y;
		shot = players[myPlayerNumber].GetShotState() ? 1 : 0;

		sw.Write(myPlayerNumber);
		sw.Write(x);
		sw.Write(y);
		sw.Write(dirX);
		sw.Write(dirY);
		sw.Write(shot);

		clientSocket->Send(outstream.GetData(), outstream.GetSize());
		players[myPlayerNumber].SetMoveState(false);
		players[myPlayerNumber].SetShotState(false);
	}

	if (totalPlayers == 1 && winningPlayer == -1)
	{
		for (size_t i = 0; i < players.size(); i++)
		{
			if (players[i].IsAlive() && players[i].IsActive())
				winningPlayer = (int)i + 1;
		}
		endDelay = 10.0f;
	}
	if (winningPlayer != -1)
	{
		endDelay -= deltaTime;
		if (endDelay <= 0.0f)
		{
			currentNetworkState = NetworkState::Connect;
			GameReset();
		}
		std::string playerText = "Player " + std::to_string(winningPlayer) + " Wins!";
		BatchRenderer::Get()->AddScreenText(playerText.c_str(), screenWidth * 0.5f - 120.0f, screenHeight * 0.5f - 30.0f, 30.0f, Colors::AliceBlue);
		BatchRenderer::Get()->AddScreenText("You will be disconnected from the", screenWidth * 0.5f - 250.0f, screenHeight* 0.5f, 30.0f, Colors::AliceBlue);
		playerText = " server in " + std::to_string((int)endDelay) + " seconds.";
		BatchRenderer::Get()->AddScreenText(playerText.c_str(), screenWidth * 0.5f - 170.0f, screenHeight * 0.5f + 30.0f, 30.0f, Colors::AliceBlue);
	}
}

void GameState::ConnectToServer(float deltaTime)
{
	auto screenWidth = GraphicsSystem::Get()->GetBackBufferWidth();
	auto screenHeight = GraphicsSystem::Get()->GetBackBufferHeight();
	auto inputSystem = InputSystem::Get();

	Vector2 bgPosition = { 0.0f, static_cast<float>(screenHeight) };
	BatchRenderer::Get()->AddSprite(background, bgPosition, Pivot::BottomLeft);

	Vector2 targetPosition = { static_cast<float>(inputSystem->GetMouseScreenX()),static_cast<float>(inputSystem->GetMouseScreenY()) };
	BatchRenderer::Get()->AddSprite(mCrossHairTexture, targetPosition);

	BatchRenderer::Get()->AddScreenText("Networked Shooter", screenWidth * 0.5f - 270.0f, screenHeight * 0.5f - 310.0f, 50.0f, Colors::AliceBlue);
	BatchRenderer::Get()->AddScreenText("Last player alive wins", screenWidth * 0.5f - 140.0f, screenHeight * 0.5f - 250.0f, 20.0f, Colors::AliceBlue);
	BatchRenderer::Get()->AddScreenText("2-4 players", screenWidth * 0.5f - 90.0f, screenHeight * 0.5f - 230.0f, 20.0f, Colors::AliceBlue);
	BatchRenderer::Get()->AddScreenText("When all players are ready, the game will start", screenWidth * 0.5f - 280.0f, screenHeight * 0.5f - 210.0f, 20.0f, Colors::AliceBlue);

	if (!connectedToServer)
	{
		BatchRenderer::Get()->AddScreenText("Press S to connect to the Server", screenWidth * 0.5f - 280.0f, screenHeight * 0.5f - 30.0f, 30.0f, Colors::AliceBlue);
	}
	else if (connectedToServer && !isReady)
	{
		std::string playerText = "You are player " + std::to_string(myPlayerNumber + 1);
		BatchRenderer::Get()->AddScreenText("Connected to the Server", screenWidth * 0.5f - 230.0f, screenHeight * 0.5f - 30.0f, 30.0f, Colors::AliceBlue);
		BatchRenderer::Get()->AddScreenText(playerText.c_str(), screenWidth * 0.5f - 170.0f, screenHeight * 0.5f, 30.0f, Colors::AliceBlue);
		BatchRenderer::Get()->AddScreenText("Press R when ready", screenWidth* 0.5f - 180.0f, screenHeight * 0.5f + 30.0f, 30.0f, Colors::AliceBlue);
	}
	else if (connectedToServer && isReady)
	{
		BatchRenderer::Get()->AddScreenText("You are Ready", screenWidth* 0.5f - 130.0f, screenHeight * 0.5f - 30.0f, 30.0f, Colors::AliceBlue);
		BatchRenderer::Get()->AddScreenText("Waiting for all other players", screenWidth * 0.5f - 240.0f, screenHeight * 0.5f, 30.0f, Colors::AliceBlue);
		BatchRenderer::Get()->AddScreenText("Game will start when all connected players are ready", screenWidth * 0.5f - 400.0f, screenHeight * 0.5f + 30.0f, 30.0f, Colors::AliceBlue);

		uint8_t buffer[1024];
		int byteReceived = clientSocket->Receive(buffer, static_cast<int>(std::size(buffer)));
		if (byteReceived > 0)
		{
			// Check which players are conncted
			Network::MemoryStream inStream(buffer, byteReceived);
			Network::StreamReader sr(inStream);

			sr.Read(totalPlayers);

			for (int i = 0; i < totalPlayers; i++)
				players[i].SetActive(true);
			currentNetworkState = NetworkState::GamePlay;
			ClientInit();
		}
	}

	if (inputSystem->IsKeyPressed(KeyCode::S) && !connectedToServer)
	{
		// Connect and Get Player Number
		Network::SocketAddress serverInfo("127.0.0.1", 8888);
		connectedToServer = clientSocket->Connect(serverInfo);
		if (connectedToServer)
		{
			uint8_t buffer[1024];
			int byteReceived = clientSocket->Receive(buffer, static_cast<int>(std::size(buffer)));
			if (byteReceived > 0)
			{
				// Grab data from packet
				Network::MemoryStream inStream(buffer, byteReceived);
				Network::StreamReader sr(inStream);
				sr.Read(myPlayerNumber);
				players[myPlayerNumber].SetSelectedPlayer(true);
			}
		}
	}
	if (inputSystem->IsKeyPressed(KeyCode::R) && connectedToServer && !isReady)
	{
		isReady = true;

		bool ready = true;
		Network::MemoryStream outstream(MAXRECVSIZE);
		Network::StreamWriter sw(outstream);
		sw.Write(ready);

		int bytes = clientSocket->Send(outstream.GetData(), outstream.GetSize());
		clientSocket->SetNonBlocking(true);
	}
}

void GameState::ClientInit()
{
	clientSocket->SetNonBlocking(true);
	clientSocket->SetNoDelay(true);
	stopThread = false;
	receiveThread = std::thread(&GameState::ProcessReceive, this);
}

void GameState::GameReset()
{
	if (receiveThread.joinable())
	{
		stopThread = true;
		receiveThread.join();
	}

	int ready = -1;
	Network::MemoryStream outstream(MAXRECVSIZE);
	Network::StreamWriter sw(outstream);
	sw.Write(ready);
	int bytes = clientSocket->Send(outstream.GetData(), outstream.GetSize());

	clientSocket->Close();
	clientSocket->Open();
	endDelay = 0.0f;
	totalPlayers = 0;
	myPlayerNumber = 0;
	winningPlayer = -1;
	connectedToServer = false;
	isReady = false;

	for (auto& player : players)
		player.Reset();
}

void GameState::Render()
{
	switch (currentNetworkState)
	{
	case NetworkState::Connect:
		break;
	case NetworkState::GamePlay:
		for (auto& player : players)
		{
			if (player.IsActive())
				player.Render();
		}
		break;
	default:
		break;
	}
}

void GameState::DebugUI()
{
	SimpleDraw::Render(mCamera);
}