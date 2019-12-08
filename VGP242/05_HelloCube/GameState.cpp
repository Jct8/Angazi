#include "GameState.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::LightGray);

	mCamera.SetPosition({ 0.0f,0.0f,-5.0f });
	mCamera.SetDirection({ 0.0f,0.0f, 1.0f });

	//Front Vertices
	mMesh.mVertices.emplace_back(Vertex{ Vector3{ -0.5f,  0.5f, 0.0f } , Colors::Gold });
	mMesh.mVertices.emplace_back(Vertex{ Vector3{  0.5f,  0.5f, 0.0f } , Colors::Purple });
	mMesh.mVertices.emplace_back(Vertex{ Vector3{  0.5f, -0.5f, 0.0f } , Colors::Gold });
	mMesh.mVertices.emplace_back(Vertex{ Vector3{ -0.5f, -0.5f, 0.0f } , Colors::Black });

	//Back Vertices
	mMesh.mVertices.emplace_back(Vertex{ Vector3{ -0.5f,  0.5f, 1.0f } , Colors::Gold });
	mMesh.mVertices.emplace_back(Vertex{ Vector3{  0.5f,  0.5f, 1.0f } , Colors::Purple });
	mMesh.mVertices.emplace_back(Vertex{ Vector3{  0.5f, -0.5f, 1.0f } , Colors::Black });
	mMesh.mVertices.emplace_back(Vertex{ Vector3{ -0.5f, -0.5f, 1.0f } , Colors::Gold });

	////Front
	//mMesh.mIndices.push_back(0);
	//mMesh.mIndices.push_back(1);
	//mMesh.mIndices.push_back(2);

	//mMesh.mIndices.push_back(0);
	//mMesh.mIndices.push_back(2);
	//mMesh.mIndices.push_back(3);

	////Back
	//mMesh.mIndices.push_back(4);
	//mMesh.mIndices.push_back(6);
	//mMesh.mIndices.push_back(5);

	//mMesh.mIndices.push_back(4);
	//mMesh.mIndices.push_back(7);
	//mMesh.mIndices.push_back(6);


	//Triangle Strip
	//Front
	mMesh.mIndices.push_back(0);
	mMesh.mIndices.push_back(1);
	mMesh.mIndices.push_back(3);
	mMesh.mIndices.push_back(2);

	//Bottom 1
	mMesh.mIndices.push_back(6);
	//Right
	mMesh.mIndices.push_back(1);
	mMesh.mIndices.push_back(5);

	//Top
	mMesh.mIndices.push_back(0);
	mMesh.mIndices.push_back(4);
	
	//Left
	mMesh.mIndices.push_back(3);
	mMesh.mIndices.push_back(7);

	//Bottom 2
	mMesh.mIndices.push_back(6);

	//Back
	mMesh.mIndices.push_back(4);
	mMesh.mIndices.push_back(5);

	mConstantBuffer.Initialize(sizeof(Matrix4));
	mMeshBuffer.Initialize(
		mMesh.mVertices.data(), mMesh.mVertices.size(),
		mMesh.mIndices.data(), mMesh.mIndices.size());

	mVertexShader.Initialize();
	mPixelShader.Initialize();

}

void GameState::Terminate()
{
	mMeshBuffer.Terminate();
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
	mCamera.Yaw(inputSystem->GetMouseMoveX() *kTurnSpeed*deltaTime);
	mCamera.Pitch(inputSystem->GetMouseMoveY() *kTurnSpeed*deltaTime);

	if (inputSystem->IsKeyDown(KeyCode::A))
		mRotation += deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::D))
		mRotation -= deltaTime;
}

void GameState::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();

	float rot = mRotation;

	for (int i = 1; i < 3; i++)
	{
		auto matWorld = Matrix4::RotationY(rot*i*3.14);
		auto matView = mCamera.GetViewMatrix();
		auto matProj = mCamera.GetPerspectiveMatrix();
		auto matWVP = Transpose(matWorld * matView * matProj);

		mConstantBuffer.Set(&matWVP);
		mConstantBuffer.Bind();

		mVertexShader.Bind();
		mPixelShader.Bind();
		mMeshBuffer.Draw();
	}

	

	//context->Draw(mVertices.size(), 0); <- this is for when we dont have an index buffer
	//context->DrawIndexed((UINT)mIndices.size(), 0, 0);

}