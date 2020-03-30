#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mAISettings.worldSize.x = static_cast<float>(GraphicsSystem::Get()->GetBackBufferWidth());
	mAISettings.worldSize.y = static_cast<float>(GraphicsSystem::Get()->GetBackBufferHeight());
	mAISettings.partitionGridSize = 100.0f;

	mAIWorld.Initialize(mAISettings);

	mPlayer = std::make_unique<Player>(mAIWorld);
	for (int i = 0; i < maxEneimes; i++)
	{
		enemies.emplace_back(std::make_unique<Enemy>(mAIWorld));
		enemies[i]->Load();
		enemies[i]->threat = mPlayer.get();
		enemies[i]->position = Math::RandomVector2({ 0.0f,0.0f }, { mAISettings.worldSize.x ,mAISettings.worldSize.y });
	}

	mPlayer->Load();

	mAIWorld.AddObstacles({ {300.0f,300.0f}, 100.0f });
	mAIWorld.AddObstacles({ {800.0f,200.0f}, 80.0f });
	mAIWorld.AddObstacles({ {600.0f,500.0f}, 100.0f });

	mPlayer->threat = enemies[0].get();
}

void GameState::Terminate()
{
	for (int i = 0; i < maxEneimes; i++)
	{
		enemies[i]->Unload();
		enemies[i].reset();
	}
	mPlayer->Unload();
	mPlayer.reset();
}

void GameState::Update(float deltaTime)
{
	for (auto &enemy : enemies)
	{
		enemy->Update(deltaTime);
	}

	mAIWorld.Update();
	mAIWorld.DebugDraw();

	mPlayer->Update(deltaTime);
}

void GameState::Render()
{
	SpriteRenderer::Get()->BeginRender();
	for (auto &enemy : enemies)
	{
		enemy->Render();
	}
	mPlayer->Render();
	SpriteRenderer::Get()->EndRender();

	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	EnemyDebugUI();
	mPlayer->DebugUI();
}

void GameState::EnemyDebugUI()
{
	ImGui::Begin("Options", nullptr, ImGuiWindowFlags_NoResize);
	ImGui::BeginGroup();
	if (ImGui::CollapsingHeader("Enemy Behavior"))
	{
		static bool isSeeking = false;
		static bool isFleeing = false;
		static bool isWandering = false;
		static bool isHiding = false;
		static bool isAvoidObstacles = false;
		static bool isCohesion = false;
		static bool isAlignment = false;
		static bool isSeperating = false;

		if (ImGui::Checkbox("Seeking ", &isSeeking))
		{
			for (auto &enemy : enemies)
			{
				enemy->velocity = { 0.0f,0.0f };
				enemy->SetSeekingBehavior(isSeeking);
			}
		}
		if (ImGui::Checkbox("Fleeing", &isFleeing))
		{
			for (auto &enemy : enemies)
			{
				enemy->velocity = { 0.0f,0.0f };
				enemy->SetFleeingBehavior(isFleeing);
			}
		}
		if (ImGui::Checkbox("Wandering", &isWandering))
		{
			for (auto &enemy : enemies)
			{
				enemy->velocity = { 0.0f,0.0f };
				enemy->SetWanderBehavior(isWandering);
			}
		}
		if (ImGui::Checkbox("Hiding", &isHiding))
		{
			for (auto &enemy : enemies)
			{
				enemy->velocity = { 0.0f,0.0f };
				enemy->SetHidingBehavior(isHiding);
			}
		}
		if (ImGui::Checkbox("Avoid Obstacles", &isAvoidObstacles))
		{
			for (auto &enemy : enemies)
			{
				enemy->velocity = { 0.0f,0.0f };
				enemy->SetObstacleAvoidBehavior(isAvoidObstacles);
			}
		}
		if (ImGui::Checkbox("Cohesion", &isCohesion))
		{
			for (auto &enemy : enemies)
			{
				enemy->velocity = { 0.0f,0.0f };
				enemy->SetCohesionBehavior(isCohesion);
			}
		}
		if (ImGui::Checkbox("Alignment", &isAlignment))
		{
			for (auto &enemy : enemies)
			{
				enemy->velocity = { 0.0f,0.0f };
				enemy->SetAlignmentBehavior(isAlignment);
			}
		}
		if (ImGui::Checkbox("Seperating", &isSeperating))
		{
			for (auto &enemy : enemies)
			{
				enemy->velocity = { 0.0f,0.0f };
				enemy->SetSeperationBehavior(isSeperating);
			}
		}

	}
	ImGui::EndGroup();

	ImGui::BeginGroup();
	if (ImGui::CollapsingHeader("Debug Options"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		static bool isDebugging = false;
		if (ImGui::Checkbox("Enemy Debug Options", &isDebugging), ImGuiTreeNodeFlags_DefaultOpen)
		{
			for (auto &enemy : enemies)
			{
				enemy->isDebug = isDebugging;
			}
		}
	}
	ImGui::EndGroup();

	ImGui::BeginGroup();
	if (ImGui::CollapsingHeader("Speed Parameters"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		static float maxSpeed = 500.0f;
		if (ImGui::SliderFloat("Enemy Speed", &maxSpeed, 10.0f, 1000.0f))
		{
			for (auto &enemy : enemies)
			{
				enemy->maxSpeed = maxSpeed;
			}
		}
	}
	ImGui::EndGroup();

	ImGui::End();

}