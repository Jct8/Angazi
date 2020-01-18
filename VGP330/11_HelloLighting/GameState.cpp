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

	mMeshSphere = MeshBuilder::CreateSpherePN();

	mConstantBuffer.Initialize(sizeof(Matrix4));

	mVertexShader.Initialize("../../Assets/Shaders/DoTexturing.fx", VertexPN::Format);
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

	SimpleDraw::AddSphere(2.0f,Colors::Gold,true);
}

void GameState::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();

	auto matWorld = Matrix4::RotationY(mRotation.x);
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();
	mConstantBuffer.Bind();

	mVertexShader.Bind();
	mPixelShader.Bind();

	auto matWVP = Transpose(matWorld * matView * matProj);

	mConstantBuffer.Set(&matWVP);
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	
}