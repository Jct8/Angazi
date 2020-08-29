#pragma once
#include <Angazi/Inc/Angazi.h>
#include <thread>

class GameState : public Angazi::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	
	void DebugUI() override;

public:
	void ProcessReceive();
	void RunGamePlay(float deltaTime);
	void ConnectToServer(float deltaTime);
	void ClientInit();
	void GameReset();

private:
	Angazi::Graphics::Camera mCamera;

	Angazi::Graphics::TextureId mCrossHairTexture = 0;
	Angazi::Graphics::TextureId background = 0;

	std::unique_ptr<Angazi::Network::TcpSocket> clientSocket;
	std::thread receiveThread;

	float endDelay = 0.0f;

	int myPlayerNumber = 0;
	int winningPlayer = -1;

	bool connectedToServer = false;
	bool isReady = false;
	bool stopThread = false;

};