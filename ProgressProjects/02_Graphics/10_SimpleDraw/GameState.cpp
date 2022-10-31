#include "GameState.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::LightGray);

	mCamera.SetPosition({ 0.0f, 0.0f,-1.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });
	mCamera.SetFov(60.0f * Math::Constants::DegToRad);
	mCamera.SetNearPlane(0.01f);
	mCamera.SetFarPlane(10000.0f);

	mConstantBuffer.Initialize(sizeof(Matrix4));

	mVertexShader.Initialize(Angazi::Core::FilePath::GetAssetFilePath() + "Shaders/DoTexturing.fx", VertexPX::Format);
	mPixelShader.Initialize(Angazi::Core::FilePath::GetAssetFilePath() + "Shaders/DoTexturing.fx");
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
	SimpleDraw::AddCylinder({ 0.0f,0.0f,0.0f }, { 0.0f, 0.0f, 1.0f }, 5.0f, 0.05f, Colors::Red, true);
	SimpleDraw::AddCone({ 0.0f,0.0f,5.0f }, { 0.0f, 0.0f, 1.0f }, 0.5f, 0.1f, Colors::Red, true);

	SimpleDraw::AddLine(Math::Vector3{ 0.0f,0.0f,0.0f }, Math::Vector3{ 0.0f,5.0f,0.0f }, Colors::Green);
	SimpleDraw::AddCylinder({ 0.0f,0.0f,0.0f }, { 0.0f, 1.0f, 0.0f }, 5.0f, 0.05f, Colors::Green, true);
	SimpleDraw::AddCone({ 0.0f,5.0f,0.0f }, { 0.0f, 1.0f, 0.0f }, 0.5f, 0.1f, Colors::Green, true);

	SimpleDraw::AddLine(Math::Vector3{ 0.0f, 0.0f, 0.0f }, Math::Vector3{ 5.0f,0.0f,0.0f }, Colors::Blue);
	SimpleDraw::AddCylinder({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, 5.0f, 0.05f, Colors::Blue, true);
	SimpleDraw::AddCone({ 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, 0.5f, 0.1f, Colors::Blue, true);

	//SimpleDraw::AddAABB({ 5.0,5.0f,5.0f }, { 2.0f, 3.0f, 4.0f }, Colors::AliceBlue, false);
	//SimpleDraw::AddCone(2.0f, 1.0f, Colors::MintCream, true);
	SimpleDraw::AddCone({ 0.0f,0.0f,0.0f }, {-6.0f,-2.0f,-5.0f}, 2.0f, 1.0f, Colors::MintCream, false);
	SimpleDraw::AddCylinder({ 1.0f,0.0f,0.0f }, {25.0f, -10.0f, 20.0f}, 2.0f, 0.2f, Colors::Green, true);
	SimpleDraw::AddSphere({ 5.0f , 5.0f,5.0f }, 4.0f, Colors::Black, false, 8, 16);
	//SimpleDraw::AddSphere( 5.0f , 5.0f,5.0f , 4.0f, Colors::Black, false, 32, 32);
	//SimpleDraw::AddSphere(Sphere{ { 5.0f , 5.0f,5.0f } , 4.0f }, Colors::Black, false, 32, 32);
	//SimpleDraw::AddGroundPlane(20.0f);

	SimpleDraw::AddScreenLine({ 0.0f , 0.0f }, { 50.0f,50.0f }, Colors::AliceBlue);
	SimpleDraw::AddScreenCircle({ 50.0f, 50.0f }, 10.0f, Colors::AliceBlue);
	SimpleDraw::AddScreenRect({ 50.0f, 50.0f }, { 100.0f, 100.0f }, Colors::AliceBlue);
}

void GameState::Render()
{
	auto matWorld = Matrix4::RotationY(mRotation);
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();
	mConstantBuffer.BindVS();

	mVertexShader.Bind();
	mPixelShader.Bind();

	auto matWVP = Transpose(matWorld * matView * matProj);

	mConstantBuffer.Update(&matWVP);
	SimpleDraw::Render(mCamera);
}