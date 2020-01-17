#include "GameState.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::LightGray);

	mCamera.SetPosition({ 0.0f, 0.0f,0.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });
	mCamera.SetFov(60.0f * Math::Constants::DegToRad);
	mCamera.SetNearPlane(0.01f);
	mCamera.SetFarPlane(10000.0f);

	mConstantBuffer.Initialize(sizeof(Matrix4));

	mVertexShader.Initialize("../../Assets/Shaders/DoTexturing.fx", VertexPX::Format);
	mPixelShader.Initialize("../../Assets/Shaders/DoTexturing.fx");
}

void GameState::Terminate()
{
	mConstantBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
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
		mCamera.Strafe(-kMoveSpeed * deltaTime);
	//mRotation += deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(kMoveSpeed*deltaTime);
	//mRotation -= deltaTime;

	SimpleDraw::AddLine(Math::Vector3{ 0.0f,0.0f,0.0f }, Math::Vector3{ 0.0f,0.0f,5.0f }, Colors::Red);
	SimpleDraw::AddLine(Math::Vector3{ 0.0f,0.0f,0.0f }, Math::Vector3{ 0.0f,5.0f,0.0f }, Colors::Green);
	SimpleDraw::AddLine(Math::Vector3{ 0.0f,0.0f,0.0f }, Math::Vector3{ 5.0f,0.0f,0.0f }, Colors::Blue);
	SimpleDraw::AddBox(2.0f, 3.0f, 4.0f, Colors::AliceBlue);
	SimpleDraw::AddCone(2.0f, 1.0f, Colors::MintCream);
	SimpleDraw::AddSphere(2.0f,Colors::Black);
	SimpleDraw::AddLine({ 0,0 }, { 0,1 }, Colors::AliceBlue);
}

void GameState::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();

	auto matWorld = Matrix4::RotationY(mRotation);
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();
	mConstantBuffer.Bind();

	mVertexShader.Bind();
	mPixelShader.Bind();

	auto matWVP = Transpose(matWorld * matView * matProj);

	mConstantBuffer.Set(&matWVP);
	SimpleDraw::Render(mCamera);
}