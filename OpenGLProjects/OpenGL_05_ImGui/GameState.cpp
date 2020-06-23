#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f,0.0f,-5.0f });
	mCamera.SetDirection({ 0.0f,0.0f, 1.0f });

	mMeshBuffer.Initialize(MeshBuilder::CreateCubePX());

	mVertexShader.Initialize("../../Assets/GLShaders/Camera.glsl",VertexPX::Format);
	mPixelShader.Initialize("../../Assets/GLShaders/Camera.glsl");

	mTexture.Initialize("../../Assets/Images/Goat.jpg");
	mTransformBuffer.Initialize();
}

void GameState::Terminate()
{
	mTransformBuffer.Terminate();
	mTexture.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float kTurnSpeed = 10.0f * Constants::DegToRad;

	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(kMoveSpeed*deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(kMoveSpeed*deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() *kTurnSpeed*deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() *kTurnSpeed*deltaTime);
	}

	//if (inputSystem->IsKeyDown(KeyCode::A))
	//	mRotation += deltaTime;
	//if (inputSystem->IsKeyDown(KeyCode::D))
	//	mRotation -= deltaTime;
}

void GameState::Render()
{
	auto matWorld = Matrix4::RotationY(mRotation);
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	mVertexShader.Bind();
	mPixelShader.Bind();
	mTexture.BindPS();

	auto matWVP = Transpose(matWorld * matView * matProj);
	data.wvp = matWVP;
	mTransformBuffer.Set(data);
	mTransformBuffer.BindVS(0);

	mMeshBuffer.Draw(); 
}

void GameState::DebugUI()
{
	ImGui::ShowDemoWindow();
}