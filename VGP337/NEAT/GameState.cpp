#include "GameState.h"
#include "ImGui/Inc/imgui.h"

#include "Bird.h"
#include "Pipe.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

namespace
{
	std::vector<Bird> birds;
	std::vector<Pipe> pipes;
	float spawnDelay = 0.0f;
	bool run = false;

	void Title(float deltaTime);
	void Play(float deltaTime);
	std::function<void(float)> Tick = Title;

	void Title(float deltaTime)
	{
		BatchRender::Get()->AddScreenText("Hit [Space] to Flap", 10.0f, 10.0f, 20.0f, Colors::White);
		BatchRender::Get()->AddScreenText("Hit [N] to NEAT", 10.0f, 30.0f, 20.0f, Colors::White);

		if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
		{
			auto& bird = birds.emplace_back();
			bird.SetPosition({ 150.0f, 300.0f });

			Tick = Play;
		}
		else if (InputSystem::Get()->IsKeyPressed(KeyCode::N))
		{
			auto& bird = birds.emplace_back();
			bird.SetPosition({ 150.0f, 300.0f });

			auto& bird2 = birds.emplace_back();
			bird2.SetPosition({ 150.0f, 500.0f });

			Tick = Play;
		}
	}

	void Play(float deltaTime)
	{
		spawnDelay -= deltaTime;
		if (spawnDelay <= 0.0f)
		{
			auto& pipe = pipes.emplace_back();
			pipe.Spawn(100.0f);
			spawnDelay = 5.0f;
		}

		for (auto& pipe : pipes)
			pipe.Update(deltaTime);

		auto IsOffScreen = [](const auto& pipe) { return pipe.GetTopRect().right < 0.0f; };
		pipes.erase(std::remove_if(pipes.begin(), pipes.end(), IsOffScreen), pipes.end());

		for (auto& bird : birds)
			bird.Update(deltaTime);

		if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
			birds[0].Flap();

		for (auto& pipe : pipes)
			pipe.Render();
		for (auto& bird : birds)
			bird.Render();
	}

}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mBackgroundTexture = TextureManager::Get()->Load("FlappyBird/bg.jpg");
}

void GameState::Terminate()
{
	mBackgroundTexture = 0;
}

void GameState::Update(float deltaTime)
{
	Tick(deltaTime);
}

void GameState::Render()
{
	auto height = GraphicsSystem::Get()->GetBackBufferHeight() *0.5f;
	auto width = GraphicsSystem::Get()->GetBackBufferWidth() *0.5f;
	BatchRender::Get()->AddSprite(mBackgroundTexture, { width,height });
}

void GameState::DebugUI()
{
	SimpleDraw::Render(mCamera);
}
