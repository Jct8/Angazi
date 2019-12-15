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
	//
	mMesh.vertices.push_back({ Vector3{ -0.5f,  0.5f, 1.0f } , Colors::Gold });
	mMesh.vertices.push_back({ Vector3{  0.5f,  0.5f, 1.0f } , Colors::Purple });
	mMesh.vertices.push_back({ Vector3{  0.5f, -0.5f, 1.0f } , Colors::Black });
	mMesh.vertices.push_back({ Vector3{ -0.5f, -0.5f, 1.0f } , Colors::Gold });

	//Homework
	// Update HellowTexuring to use a MeshPC data to draw texture mapped cubes
	// You will need to add sampler and Texture classes provided
	// You will need to use DoTexturing.fx shaders
	// Add a new class called Graphics::MeshBuilder with the following functions

	//namespace Angazi::Graphics
	/*{
		class Meshbuilder
		{
		public: 
			static MeshPX CreatePlanePX();
			static MeshPX CreateCylinderPX();
			static MeshPX CreateSpherePX(float radius,int rings = 16, int slices = 16);
		};
	}*/

	//This will allow you ti create a mesh easily by doing
	//	auto mesh = MeshBuilder::CreateSpherePX(...);

	// Add HelloEarth to test a texture mapped sphere using Earth Texture

	// A plane:
	/*for (int y = 0; y < height; y++)
	{
		for (int i = 0; i < width; i++)
		{
			vertices.push_back({x,y,0.0f}...);
		}
	}*/

	// A cylinder:
	/*for (int y = 0; y < height; y++)
	{
		for (int theta = 0; theta < TwoPi; theta+= ...)
		{
			vertices.push_back({sin(theta),y,cos(theta)}...);
		}
	}*/

	// A sphere:
	/*for (int phi = 0; phi < Pi; phi+=)
	{
		for (int theta = 0; theta < TwoPi; theta+= ...)
		{
			vertices.push_back({sin(theta) * r ,y,cos(theta)*r}...);
		}
	}*/

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

	mVertexShader.Initialize("../../Assets/Shaders/DoTexturing.fx",VertexPC::Format);
	mPixelShader.Initialize("../../Assets/Shaders/DoTexturing.fx");

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
	auto context = GraphicsSystem::Get()->GetContext();

	auto matWorld = Matrix4::RotationY(mRotation);
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();
	mConstantBuffer.Bind();

	mVertexShader.Bind();
	mPixelShader.Bind();

	for (int i = 0; i < 50; i++)
	{
		auto translation = Matrix4::Translation({ i*2.0f,i*1.0f,i* 4.0f });
		auto matWVP = Transpose(translation *matWorld * matView * matProj);

		mConstantBuffer.Set(&matWVP);
		mMeshBuffer.Draw();
	}



	//context->Draw(mVertices.size(), 0); <- this is for when we dont have an index buffer
	//context->DrawIndexed((UINT)mIndices.size(), 0, 0);

}