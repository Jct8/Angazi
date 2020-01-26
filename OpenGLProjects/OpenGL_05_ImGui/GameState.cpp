#include "GameState.h"

using namespace Angazi;
using namespace Angazi::GraphicsGL;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f,0.0f,-5.0f });
	mCamera.SetDirection({ 0.0f,0.0f, 1.0f });

	//Front Vertices
	mMesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, 0.0f } , Vector2{0.0f , 0.0f }});
	mMesh.vertices.push_back({ Vector3{  0.5f,  0.5f, 0.0f } , Vector2{1.0f , 0.0f }});
	mMesh.vertices.push_back({ Vector3{  0.5f, -0.5f, 0.0f } , Vector2{1.0f , 1.0f }});
	mMesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, 0.0f } , Vector2{0.0f , 1.0f }});
	//Back
	mMesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, 1.0f }  ,Vector2{ 0.0f , 0.0f} });
	mMesh.vertices.push_back({ Vector3{  0.5f,  0.5f, 1.0f }  ,Vector2{ 1.0f , 0.0f} });
	mMesh.vertices.push_back({ Vector3{  0.5f, -0.5f, 1.0f }  ,Vector2{ 1.0f , 1.0f} });
	mMesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, 1.0f }  ,Vector2{ 0.0f , 1.0f} });

	//Front
	mMesh.indices.push_back(2);
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(0);

	mMesh.indices.push_back(3);
	mMesh.indices.push_back(2);
	mMesh.indices.push_back(0);

	//Back
	mMesh.indices.push_back(5);
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(4);

	mMesh.indices.push_back(6);
	mMesh.indices.push_back(7);
	mMesh.indices.push_back(4);

	//Left
	mMesh.indices.push_back(3);
	mMesh.indices.push_back(0);
	mMesh.indices.push_back(4);

	mMesh.indices.push_back(7);
	mMesh.indices.push_back(3);
	mMesh.indices.push_back(4);

	//Right
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(5);
	mMesh.indices.push_back(1);

	mMesh.indices.push_back(2);
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(1);

	//Top
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(5);
	mMesh.indices.push_back(4);

	mMesh.indices.push_back(0);
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(4);

	//Bottom
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(2);
	mMesh.indices.push_back(3);

	mMesh.indices.push_back(7);
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(3);

	mMeshBuffer.Initialize(mMesh, VertexPX::Format);

	mShader.Initialize("../../Assets/Shaders/GLCamera.glsl");

	mTexture.Initialize("Goat.jpg");
}

void GameState::Terminate()
{
	mTexture.Terminate();
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
}

void GameState::Render()
{
	auto matWorld = Matrix4::RotationY(mRotation);
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	mShader.Bind();
	mTexture.Bind();

	auto matWVP = Transpose(matWorld * matView * matProj);
	mShader.SetUniformMat4f("WVP", matWVP);

	mMeshBuffer.Draw();
}

#include "ImGui/Inc/imgui.h"
void GameState::DebugUI()
{
	ImGui::ShowDemoWindow();
}