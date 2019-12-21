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
	mMesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, 0.0f } , 0.0f , 0.0f });
	mMesh.vertices.push_back({ Vector3{  0.5f,  0.5f, 0.0f } , 1.0f , 0.0f });
	mMesh.vertices.push_back({ Vector3{  0.5f, -0.5f, 0.0f } , 1.0f , 1.0f });
	mMesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, 0.0f } , 0.0f , 1.0f });
	//Back
	mMesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, 1.0f }  , 0.0f , 0.0f });
	mMesh.vertices.push_back({ Vector3{  0.5f,  0.5f, 1.0f }  , 1.0f , 0.0f });
	mMesh.vertices.push_back({ Vector3{  0.5f, -0.5f, 1.0f }  , 1.0f , 1.0f });
	mMesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, 1.0f }  , 0.0f , 1.0f });

	//Front
	mMesh.indices.push_back(0);
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(2);

	mMesh.indices.push_back(0);
	mMesh.indices.push_back(2);
	mMesh.indices.push_back(3);

	//Back
	mMesh.indices.push_back(4);
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(5);

	mMesh.indices.push_back(4);
	mMesh.indices.push_back(7);
	mMesh.indices.push_back(6);

	//Left
	mMesh.indices.push_back(4);
	mMesh.indices.push_back(0);
	mMesh.indices.push_back(3);

	mMesh.indices.push_back(4);
	mMesh.indices.push_back(3);
	mMesh.indices.push_back(7);

	//Right
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(5);
	mMesh.indices.push_back(6);

	mMesh.indices.push_back(1);
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(2);

	//Top
	mMesh.indices.push_back(4);
	mMesh.indices.push_back(5);
	mMesh.indices.push_back(1);

	mMesh.indices.push_back(4);
	mMesh.indices.push_back(1);
	mMesh.indices.push_back(0);

	//Bottom
	mMesh.indices.push_back(3);
	mMesh.indices.push_back(2);
	mMesh.indices.push_back(6);

	mMesh.indices.push_back(3);
	mMesh.indices.push_back(6);
	mMesh.indices.push_back(7);

	mMeshPlane = MeshBuilder::CreatePlanePX(20, 20);
	mMeshCylinder = MeshBuilder::CreateCylinderPX(16, 3 , 16);
	mMeshSphere =  MeshBuilder::CreateSpherePX(5);
	mConstantBuffer.Initialize(sizeof(Matrix4));
	mMeshBuffer.Initialize(mMesh);
	mMeshBufferPlane.Initialize(mMeshPlane);
	mMeshBufferCylinder.Initialize(mMeshCylinder);
	mMeshBufferSphere.Initialize(mMeshSphere);

	mVertexShader.Initialize("../../Assets/Shaders/DoTexturing.fx",VertexPX::Format);
	mPixelShader.Initialize("../../Assets/Shaders/DoTexturing.fx");
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Border);
	mTexture.Initialize("GOAT.jpg");
}

void GameState::Terminate()
{
	mTexture.Terminate();
	mSampler.Terminate();
	mMeshBuffer.Terminate();
	mMeshBufferPlane.Terminate();
	mMeshBufferCylinder.Terminate();
	mMeshBufferSphere.Terminate();
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

	auto matWorld = Matrix4::RotationY(mRotation);
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();
	mConstantBuffer.Bind();

	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.Bind();
	mTexture.Bind();

	auto matWVP = Transpose(matWorld * matView * matProj);

	mConstantBuffer.Set(&matWVP);
	mMeshBuffer.Draw();
	mMeshBufferPlane.Draw();
	mMeshBufferCylinder.Draw();
	mMeshBufferSphere.Draw();

}