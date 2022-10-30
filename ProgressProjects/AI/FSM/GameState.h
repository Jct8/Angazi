#pragma once
#include <Angazi/Inc/Angazi.h>

class GameState : public Angazi::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	
	void DebugUI() override;

public:
	void GameStart();
	void GamePlay(float deltaTime);
	void GameEnd();

private:
	Angazi::Graphics::Camera mCamera;

	enum GAMESTATE
	{
		Start,
		InGame,
		End
	};

	GAMESTATE gameState;
	float ScreenHeight;
	float ScreenWidth;
};
