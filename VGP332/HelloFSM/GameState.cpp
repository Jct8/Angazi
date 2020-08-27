#include "EnemyManager.h"
#include "Survivor.h"
#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

Survivor survivor;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Green);
	ScreenWidth = static_cast<float>(GraphicsSystem::Get()->GetBackBufferWidth());
	ScreenHeight = static_cast<float>(GraphicsSystem::Get()->GetBackBufferHeight());

	EnemyManager::StaticInitialize();
	EnemyManager::Get().Load();

	survivor.Load();
	survivor.SetPosition(Math::Vector2(ScreenWidth / 2.0f - 100.0f, ScreenHeight / 2.0f));

	InputSystem::Get()->ShowSystemCursor(false);
	gameState = Start;
}

void GameState::Terminate()
{
	survivor.Unload();
	EnemyManager::StaticTerminate();
}

void GameState::Update(float deltaTime)
{
	switch (gameState)
	{
	case Start:
		GameStart();
		break;
	case InGame:
		GamePlay(deltaTime);
		break;
	case End:
		GameEnd();
		break;
	default:
		break;
	}
}

void GameState::Render()
{
	if (gameState == GAMESTATE::InGame)
	{
		SpriteRenderer::Get()->BeginRender();
		EnemyManager::Get().Render();
		survivor.Render();
		SpriteRenderer::Get()->EndRender();

		SimpleDraw::Render(mCamera);
	}
}

void GameState::DebugUI()
{
}

void GameState::GameStart()
{
	auto input = InputSystem::Get();
	BatchRender::Get()->AddScreenText("Press Enter to start", GraphicsSystem::Get()->GetBackBufferWidth() *0.5f - 200.0f,
		GraphicsSystem::Get()->GetBackBufferWidth() *0.5f, 30.0f, Colors::Blue);
	if (input->IsKeyPressed(Input::KeyCode::ENTER))
		gameState = InGame;
}

void GameState::GamePlay(float deltaTime)
{
	EnemyManager::Get().Update(deltaTime);
	survivor.Update(deltaTime);

	if (!survivor.IsAlive())
		gameState = End;

}

void GameState::GameEnd()
{
	BatchRender::Get()->AddScreenText("GameOver", ScreenWidth *0.5f - 100.0f, ScreenHeight *0.5f, 30.0f, Colors::Blue);

	std::string str = "Total zombies killed:" + std::to_string(EnemyManager::Get().GetZombieScore());
	BatchRender::Get()->AddScreenText(str.c_str(), ScreenWidth *0.5f - 200.0f, ScreenHeight *0.5f + 30.0f, 30.0f, Colors::Blue);

	BatchRender::Get()->AddScreenText("Press Enter to go back to main Menu",
		ScreenWidth *0.5f - 200.0f, ScreenHeight *0.5f + 60.0f, 30.0f, Colors::Blue);

	if (InputSystem::Get()->IsKeyPressed(Input::KeyCode::ENTER))
	{
		gameState = Start;
		EnemyManager::Get().Unload();
		survivor.ResetSurvivor();
	}
}