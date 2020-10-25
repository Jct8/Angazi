#include "GameState.h"
#include "Game.h"
#include "Camera2D.h"
#include "TileMap.h"
#include "Player.h"
#include "EnemyManager.h"
#include "UIManager.h"
#include "ProjectileManager.h"
#include "ParticleManager.h"
#include "WeaponManager.h"
#include "InputManager.h"

#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

AI::AIWorld world;
AI::AIWorld::Settings aiSettings;
Player player(world, 0);
Game gameState;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	gameState = GameStart;
	backgroundTex = TextureManager::Get()->Load("../../Assets/Images/Rougelike/Background.png");
	characterTex = TextureManager::Get()->Load( "../../Assets/Images/Rougelike/Start.png");
	character2Tex = TextureManager::Get()->Load("../../Assets/Images/Rougelike/Start2.png");
	Camera2D::StaticInitialize();

	TileMap::StaticInitialize();
	TileMap::Get().LoadMap("level_01.txt", "path.txt");
	TileMap::Get().LoadTexture("tileset_02.txt");
	TileMap::Get().SetShowEditor(true);

	aiSettings.worldSize.x = static_cast<float>(TileMap::Get().GetWorldWidth());
	aiSettings.worldSize.y = static_cast<float>(TileMap::Get().GetWorldHeight());
	aiSettings.partitionGridSize = 50.0f;

	world.Initialize(aiSettings);

	EnemyManager::StaticInitialize();
	EnemyManager::Get().Load(world);

	UIManager::StaticInitialize();
	UIManager::Get().Load();

	ProjectileManager::StaticInitialize();
	ProjectileManager::Get().Load();

	ParticleManager::StaticInitialize();
	ParticleManager::Get().Load();

	WeaponManager::StaticInitialize();
	WeaponManager::Get().Load();

	InputManager::StaticInitialize();

	player.Load("PlayerTextures.txt", true);
	player.SetDestination({ 153.0f,74.0f });
	player.SetPosition({ 150.0f, 450.0f });
	player.InitializeAI();

	currentView = player.GetPosition();
	cameraOffset = GraphicsSystem::Get()->GetBackBufferWidth() *0.3f;
}

void GameState::Terminate()
{
	TileMap::Get().Unload();
	player.Unload();

	InputManager::StaticTerminate();
	WeaponManager::StaticTerminate();
	ParticleManager::StaticTerminate();
	ProjectileManager::StaticTerminate();
	UIManager::StaticTerminate();
	EnemyManager::StaticTerminate();
	TileMap::StaticTerminate();
	Camera2D::StaticTerminate();
}

void GameState::Update(float deltaTime)
{
	switch (gameState)
	{
	case GameStart:
		RunGameStart();
		break;
	case GamePlay:
		RunGamePlay(deltaTime);
		break;
	case GameWin:
		RunGameEnd();
		break;
	case GameLose:
		RunGameEnd();
		break;
	default:
		break;
	}
}

void GameState::RunGameStart()
{
	auto x = GraphicsSystem::Get()->GetBackBufferWidth()*0.5f;
	auto y = GraphicsSystem::Get()->GetBackBufferHeight()*0.5f;

	BatchRenderer::Get()->AddScreenText("Rougelike Dungeon", x - 250.0f, y - 120.0f, 60.0f, Colors::Red);
	BatchRenderer::Get()->AddScreenText("Press Space to Start", x - 200.0f, y - 50.0f, 30.0f, Colors::Red);
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::SPACE))
		gameState = GamePlay;
}

void GameState::RunGamePlay(float deltaTime)
{
	float dt = Math::Min(deltaTime, 1.0f / 60.0f);

	world.Update();
	//world.DebugDraw();
	TileMap::Get().Update(dt);
	player.Update(dt);
	EnemyManager::Get().Update(dt);

	Camera2D::Get().SetViewBound(TileMap::Get().GetBound());
	Math::Vector2 view = player.GetPosition();
	/*if (player.IsFacingLeft() && view.x - cameraOffset < currentView.x)
	{
		view.x -= cameraOffset;
		currentView = X::Math::Lerp(currentView, view, 0.01f);
	}
	else if (!player.IsFacingLeft() && view.x + cameraOffset > currentView.x)
	{
		view.x += cameraOffset;
		currentView = X::Math::Lerp(currentView, view, 0.01f);
	}*/
	Camera2D::Get().SetViewPosition(view);

	ProjectileManager::Get().Update(dt);
	ParticleManager::Get().Update(dt);

}

void GameState::RunGameEnd()
{
	auto x = GraphicsSystem::Get()->GetBackBufferWidth()*0.5f;
	auto y = GraphicsSystem::Get()->GetBackBufferHeight()*0.5f;

	if (gameState == GameWin)
		BatchRenderer::Get()->AddScreenText("You Win!\nPress Space to Restart", x - 200.0f, y - 50.0f, 30.0f, Colors::AliceBlue);
	else if (gameState == GameLose)
		BatchRenderer::Get()->AddScreenText("You Lose!\nPress Space to Restart", x - 200.0f, y - 50.0f, 30.0f, Colors::AliceBlue);
	if (Input::InputSystem::Get()->IsKeyPressed(Input::KeyCode::SPACE))
	{
		player.Reset();
		player.SetPosition({ 150.0f, 450.0f });
		EnemyManager::Get().Unload();
		EnemyManager::Get().Load(world);
		gameState = GameStart;
	}
}

void GameState::Render()
{
	auto x = GraphicsSystem::Get()->GetBackBufferWidth()*0.5f;
	auto y = GraphicsSystem::Get()->GetBackBufferHeight()*0.5f;

	switch (gameState)
	{
	case GamePlay:
		TileMap::Get().Render();
		EnemyManager::Get().Render();
		player.Render();

		ProjectileManager::Get().Render();
		ParticleManager::Get().Render();
		UIManager::Get().Render();
		break;
	default:
		BatchRenderer::Get()->AddSprite(backgroundTex, { x , y });
		BatchRenderer::Get()->AddSprite(characterTex, { x - 350.0f,y + 150.0f });
		BatchRenderer::Get()->AddSprite(character2Tex, { x + 350.0f,y - 150.0f });
		break;
	}

}

void GameState::DebugUI()
{
	SimpleDraw::Render(mCamera);
	//TileMap::Get().ShowEditor();
	player.DebugUI();
	//EnemyManager::Get().DebugUI();
}