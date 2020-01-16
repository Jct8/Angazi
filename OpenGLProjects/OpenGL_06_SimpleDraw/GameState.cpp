#include "GameState.h"

using namespace Angazi;
using namespace Angazi::GraphicsGL;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f,0.0f,-5.0f });
	mCamera.SetDirection({ 0.0f,0.0f, 1.0f });

	//mMeshBuffer.Initialize(mMesh, VertexPX::Format);

	mShader.Initialize("../../Assets/Shaders/GLCamera.glsl");
}

void GameState::Terminate()
{
	mShader.Terminate();
	mMeshBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
	const float kMoveSpeed = 10.0f;
	const float kTurnSpeed = 1.0f;

	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(kMoveSpeed*deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() *kTurnSpeed*deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() *kTurnSpeed*deltaTime);
	}

	if (inputSystem->IsKeyDown(KeyCode::A))
		mRotation += deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::D))
		mRotation -= deltaTime;

	SimpleDraw::AddLine(Math::Vector3{ 0.0f,0.0f,0.0f }, Math::Vector3{ 5.0f,0.0f,0.0f }, Colors::Blue);
	SimpleDraw::AddLine(Math::Vector3{ 0.0f,0.0f,0.0f }, Math::Vector3{ 0.0f,0.0f,5.0f }, Colors::Red);
	SimpleDraw::AddLine(Math::Vector3{ 0.0f,0.0f,0.0f }, Math::Vector3{ 0.0f,5.0f,0.0f }, Colors::Green);
	SimpleDraw::AddBox(2.0f, 3.0f, 4.0f, Colors::AliceBlue);
	SimpleDraw::AddCone(2.0f, 1.0f, Colors::MintCream);
	SimpleDraw::AddSphere(2.0f, Colors::Black);
	SimpleDraw::AddLine({ 0,0 }, { 0,1 }, Colors::AliceBlue);
}

void GameState::Render()
{
	auto matWorld = Matrix4::RotationY(mRotation);
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	mShader.Bind();

	auto matWVP = Transpose(matWorld * matView * matProj);
	mShader.SetUniformMat4f("WVP", matWVP);

	SimpleDraw::Render(mCamera);
	//mMeshBuffer.Draw();
}

#include "ImGui/Inc/imgui.h"
void GameState::DebugUI()
{
	ImGui::ShowDemoWindow();
}