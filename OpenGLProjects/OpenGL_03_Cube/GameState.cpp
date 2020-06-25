#include "GameState.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

namespace
{
	uint32_t mProgram;
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Gray);
	mCamera.SetPosition({ 0.0f,0.0f,-5.0f });
	mCamera.SetDirection({ 0.0f,0.0f, 1.0f });

	//Front Vertices
	mMesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, 0.0f } , Vector2{0.0f , 0.0f } });
	mMesh.vertices.push_back({ Vector3{  0.5f,  0.5f, 0.0f } , Vector2{1.0f , 0.0f } });
	mMesh.vertices.push_back({ Vector3{  0.5f, -0.5f, 0.0f } , Vector2{1.0f , 1.0f } });
	mMesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, 0.0f } , Vector2{0.0f , 1.0f } });
	//Back
	mMesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, 1.0f } , Vector2{0.0f , 0.0f } });
	mMesh.vertices.push_back({ Vector3{  0.5f,  0.5f, 1.0f } , Vector2{1.0f , 0.0f } });
	mMesh.vertices.push_back({ Vector3{  0.5f, -0.5f, 1.0f } , Vector2{1.0f , 1.0f } });
	mMesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, 1.0f } , Vector2{0.0f , 1.0f } });

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

	mMeshBuffer.Initialize(MeshBuilder::CreateCubePX(), VertexPX::Format);

	mVertexShader.Initialize("../../Assets/GLShaders/Camera.glsl", VertexPX::Format);
	mPixelShader.Initialize("../../Assets/GLShaders/Camera.glsl");
	//mProgram = glCreateProgram();

	//glAttachShader(mProgram, mVertexShader.GetId());
	//glAttachShader(mProgram, mPixelShader.GetId());
	//glLinkProgram(mProgram);
	//glValidateProgram(mProgram);

	mTexture.Initialize("../../Assets/Images/Goat.jpg");

	mTransformBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
}

void GameState::Terminate()
{
	mSampler.Terminate();
	mTransformBuffer.Terminate();

	mTexture.Terminate();
	//glDeleteProgram(mProgram);

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
}

void GameState::Render()
{
	auto matWorld = Matrix4::RotationY(mRotation);
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();
	auto matWVP = Transpose(matWorld * matView * matProj);

	mVertexShader.Bind();
	mPixelShader.Bind();

	mTexture.BindPS();

	mSampler.BindVS(0);
	mSampler.BindPS(0);
	data.wvp = matWVP;
	mTransformBuffer.Set(data);
	mTransformBuffer.BindVS(0);

	//glUseProgram(mProgram);

	mMeshBuffer.Draw();
}