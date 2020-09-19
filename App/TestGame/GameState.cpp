#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Physics;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCameraService = mWorld.AddService<CameraService>();
	mWorld.Initialize(100);

	auto& camera = mCameraService->GetActiveCamera();
	camera.SetNearPlane(0.1f);
	camera.SetFarPlane(300.0f);
	camera.SetPosition({ 0.0f, 10.0f, -30.0f });
	camera.SetDirection({ 0.0f,0.0f, 1.0f });

	mWorld.Create("../../Assets/Templates/test.json", "test");
	mWorld.Create("../../Assets/Templates/tallBox.json", "tall");
	mWorld.Create("../../Assets/Templates/longBox.json", "long");
	mWorld.Create("../../Assets/Templates/fatBox.json", "fat");
}

void GameState::Terminate()
{
	mWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 10.0f;
	const float kTurnSpeed = 10.0f * Constants::DegToRad;

	auto& camera = mCameraService->GetActiveCamera();
	if (inputSystem->IsKeyDown(KeyCode::W))
		camera.Walk(kMoveSpeed*deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		camera.Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		camera.Strafe(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		camera.Strafe(kMoveSpeed*deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		camera.Yaw(inputSystem->GetMouseMoveX() *kTurnSpeed*deltaTime);
		camera.Pitch(inputSystem->GetMouseMoveY() *kTurnSpeed*deltaTime);
	}

	mWorld.Update(deltaTime);
}

void GameState::Render()
{
	DrawScene();
}

void GameState::DebugUI()
{
	mWorld.DebugUI();
}

void GameState::DrawScene()
{
	mWorld.Render();

	SimpleDraw::AddGroundPlane(30);
	auto& camera = mCameraService->GetActiveCamera();

	SimpleDraw::Render(camera);
}