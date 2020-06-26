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
	mMesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, 0.0f } , Colors::Gold });
	mMesh.vertices.push_back({ Vector3{  0.5f,  0.5f, 0.0f } , Colors::Purple });
	mMesh.vertices.push_back({ Vector3{  0.5f, -0.5f, 0.0f } , Colors::Gold });
	mMesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, 0.0f } , Colors::Black });
	//Back
	mMesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, 1.0f } , Colors::Gold });
	mMesh.vertices.push_back({ Vector3{  0.5f,  0.5f, 1.0f } , Colors::Purple });
	mMesh.vertices.push_back({ Vector3{  0.5f, -0.5f, 1.0f } , Colors::Black });
	mMesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, 1.0f } , Colors::Gold });

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
	mMesh.indices.push_back(0);
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(3);
	mMesh.indices.push_back(2);

	//Bottom 1
	mMesh.indices.push_back(6);
	//Right
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(5);

	//Top
	mMesh.indices.push_back(0);
	mMesh.indices.push_back(4);

	//Left
	mMesh.indices.push_back(3);
	mMesh.indices.push_back(7);

	//Bottom 2
	mMesh.indices.push_back(6);

	//Back
	mMesh.indices.push_back(4);
	mMesh.indices.push_back(5);

	mConstantBuffer.Initialize(sizeof(Matrix4));
	mMeshBuffer.Initialize(mMesh);

	mVertexShader.Initialize("../../Assets/Shaders/DoTransform.fx", VertexPC::Format);
	mPixelShader.Initialize("../../Assets/Shaders/DoTransform.fx");

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

	//if (inputSystem->IsKeyDown(KeyCode::A))
	mRotation += deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::D))
		mRotation -= deltaTime;
}

void GameState::Render()
{
	auto matWorld = Matrix4::RotationY(mRotation);
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();
	mConstantBuffer.BindVS();

	mVertexShader.Bind();
	mPixelShader.Bind();

	mMeshBuffer.SetTopology(MeshBuffer::Topology::TrianglesStrip);
	for (int i = 0; i < 50; i++)
	{
		auto translation = Matrix4::Translation({ i*2.0f,i*1.0f,i* 4.0f });
		auto matWVP = Transpose(translation *matWorld * matView * matProj);

		mConstantBuffer.Update(&matWVP);
		mMeshBuffer.Draw();
	}



	//context->Draw(mVertices.size(), 0); <- this is for when we dont have an index buffer
	//context->DrawIndexed((UINT)mIndices.size(), 0, 0);

}