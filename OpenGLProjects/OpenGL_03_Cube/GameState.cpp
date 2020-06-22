#include "GameState.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;


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

	mMeshBuffer.Initialize(mMesh, VertexPX::Format);

	mVertexShader.Initialize("../../Assets/GLShaders/Camera.glsl", VertexPX::Format);
	mPixelShader.Initialize("../../Assets/GLShaders/Camera.glsl");

	mTexture.Initialize("../../Assets/Images/Goat.jpg");
}

void GameState::Terminate()
{
	mTexture.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
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
	mCamera.Yaw(inputSystem->GetMouseMoveX() *kTurnSpeed*deltaTime);
	mCamera.Pitch(inputSystem->GetMouseMoveY() *kTurnSpeed*deltaTime);

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
	auto matWVP = Transpose(matWorld * matView * matProj);

	auto mat = matWVP;
	GLfloat glMat[16] =
	{
		mat._11,mat._12, mat._13, mat._14,
		mat._21,mat._22, mat._23, mat._24,
		mat._31,mat._32, mat._33, mat._34,
		mat._41,mat._42, mat._43, mat._44
	};
	mVertexShader.Bind();

	glProgramUniform4fv(glGetUniformLocation(GraphicsSystem::Get()->pipeline, "WVP"), 1, GL_TRUE, glMat);
	mPixelShader.Bind();
	mTexture.BindPS();


	mMeshBuffer.Draw();
}