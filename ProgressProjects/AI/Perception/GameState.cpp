#include "GameState.h"

#include "Mineral.h"
#include "SCV.h"
#include <ImGui/Inc/imgui.h>

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

AI::AIWorld world;
SCV scv(world);

float viewRange = 300.0f;
float viewAngle = 90.0f;

float wanderRadius = 25.0f;
float wanderDistance = 100.0f;
float wanderJitter = 1.0f;

std::vector<std::unique_ptr<Mineral>> minerals;


void ShowUI()
{
	ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps: %f", Core::TimeUtil::GetFramesPerSecond());

	if (ImGui::CollapsingHeader("SCV", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("View Range##SCV", &viewRange, 1.0f, 100.0f, 1000.0f);
		ImGui::DragFloat("View Angle##SCV", &viewAngle, 1.0f, 10.0f, 180.0f);

		ImGui::DragFloat("Wander Radius##SCV", &wanderRadius, 1.0f, 1.0f, 100.0f);
		ImGui::DragFloat("Wander Distance##SCV", &wanderDistance, 1.0f, 1.0f, 500.0f);
		ImGui::DragFloat("Wander Jitter##SCV", &wanderJitter, 1.0f, 0.0f, 100.0f);
	}

	ImGui::End();
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	ScreenWidth = static_cast<float>(GraphicsSystem::Get()->GetBackBufferWidth());
	ScreenHeight = static_cast<float>(GraphicsSystem::Get()->GetBackBufferHeight());

	world.Initialize({ ScreenWidth,ScreenHeight });

	scv.Initialize();

	minerals.reserve(10);
	for (size_t i = 0; i < minerals.capacity(); ++i)
	{
		auto& m = minerals.emplace_back(std::make_unique<Mineral>(world));
		m->Initialize();
	}
}

void GameState::Terminate()
{
}

void GameState::Update(float deltaTime)
{

	world.Update();

	for (auto& m : minerals)
		m->Render();

	scv.Update(deltaTime);
	scv.Render();

}

void GameState::Render()
{
}

void GameState::DebugUI()
{
	SimpleDraw::Render(mCamera);
	ShowUI();
}

