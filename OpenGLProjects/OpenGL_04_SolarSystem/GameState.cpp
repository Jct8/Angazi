#include "GameState.h"

using namespace Angazi;
using namespace Angazi::GraphicsGL;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{
	//GraphicsSystemGL::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ 0.0f,15.0f,-40.0f });
	mCamera.SetDirection({ 0.0f,0.0f, 1.0f });

	mMeshSphere = MeshBuilder::CreateSpherePX(2, 32, 32);
	mMeshDomeSphere = MeshBuilder::CreateSpherePX(60, 32, 32,false);

	mMeshBufferSphere.Initialize(mMeshSphere,VertexPX::Format);
	mMeshBufferDome.Initialize(mMeshDomeSphere,VertexPX::Format);

	mShader.Initialize("../../Assets/GLShaders/GLCamera.glsl");

	for (int i = 0; i < 10; i++)
	{
		mPlanetTextures.emplace_back();
		mSelfRotationSpeed.emplace_back();
		mRotationSpeed.emplace_back();
		mScale.emplace_back();
		mTranslation.emplace_back();
		mMoonTextures.emplace_back();
		mMoonTextures[i].Initialize("Moon.jpg");
	}

	mPlanetTextures[0].Initialize("../../Assets/Images/Sun.jpg");
	mPlanetTextures[1].Initialize("../../Assets/Images/Mercury.jpg");
	mPlanetTextures[2].Initialize("../../Assets/Images/Venus.jpg");
	mPlanetTextures[3].Initialize("../../Assets/Images/Earth.jpg");
	mPlanetTextures[4].Initialize("../../Assets/Images/Mars.jpg");
	mPlanetTextures[5].Initialize("../../Assets/Images/Jupiter.jpg");
	mPlanetTextures[6].Initialize("../../Assets/Images/Saturn.jpg");
	mPlanetTextures[7].Initialize("../../Assets/Images/Uranus.jpg");
	mPlanetTextures[8].Initialize("../../Assets/Images/Neptune.jpg");
	mPlanetTextures[9].Initialize("../../Assets/Images/Pluto.jpg");

	mDomeTexture.Initialize("../../Assets/Images/Stars.jpg");

	mSelfRotationSpeed[0] = 0.0f;
	mSelfRotationSpeed[1] = 0.2f;
	mSelfRotationSpeed[2] = 1.4f;
	mSelfRotationSpeed[3] = 1.0f;
	mSelfRotationSpeed[4] = 1.4f;
	mSelfRotationSpeed[5] = 0.8f;
	mSelfRotationSpeed[6] = 0.4f;
	mSelfRotationSpeed[7] = 0.6f;
	mSelfRotationSpeed[8] = 1.2f;
	mSelfRotationSpeed[9] = 0.4f;

	mRotationSpeed[0] = 0.0f;
	mRotationSpeed[1] = 1.36f;
	mRotationSpeed[2] = 1.16f;
	mRotationSpeed[3] = 0.96f;
	mRotationSpeed[4] = 0.80f;
	mRotationSpeed[5] = 0.63f;
	mRotationSpeed[6] = 0.50f;
	mRotationSpeed[7] = 0.40f;
	mRotationSpeed[8] = 0.3f;
	mRotationSpeed[9] = 0.56f;

	mScale[0] = 2.0f;
	mScale[1] = 0.4f;
	mScale[2] = 0.9f;
	mScale[3] = 0.9f;
	mScale[4] = 0.6f;
	mScale[5] = 1.4f;
	mScale[6] = 1.2f;
	mScale[7] = 1.0f;
	mScale[8] = 1.0f;
	mScale[9] = 0.3f;
}

void GameState::Terminate()
{
	for (int i = 0; i < mPlanetTextures.size(); i++)
	{
		mPlanetTextures[i].Terminate();
		mMoonTextures[i].Terminate();
	}

	mShader.Terminate();
	mDomeTexture.Terminate();
	mMeshBufferSphere.Terminate();
	mMeshBufferDome.Terminate();
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

	mRotation -= deltaTime;
}

void GameState::Render()
{
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	mShader.Bind();

	for (int i = 0; i <mPlanetTextures.size(); i++)
	{
		//Planets
		auto matWorld = Matrix4::RotationY(mRotation*mRotationSpeed[i]);
		auto matSelfRotation = Matrix4::RotationY(mRotation*mSelfRotationSpeed[i]);
		auto matScale = Matrix4::Scaling(mScale[i]);
		auto matTrans = Matrix4::Translation({ 0.5f*i + i*5.0f,0.0f,0.0f });
		auto matWVP = Transpose( matScale * matSelfRotation * matTrans * matWorld * matView * matProj);

		mShader.SetUniformMat4f("WVP", matWVP);
		mPlanetTextures[i].Bind();
		mMeshBufferSphere.Draw();

		//Moon
		auto matMoon = Matrix4::RotationY(mRotation*0.4f);
		auto matMoonSelfRotation = Matrix4::RotationY(mRotation*10.5f);
		auto matMoonScale = Matrix4::Scaling(0.1f);
		auto matMoonTrans = Matrix4::Translation({ 2.0f,0.0f,0.0f });
		auto matMoonWVP = Transpose(matMoonScale *matMoonSelfRotation * matMoonTrans * matMoon * matTrans * matWorld * matView * matProj);
		
		mMoonTextures[i].Bind();
		mShader.SetUniformMat4f("WVP", matMoonWVP);
		mMeshBufferSphere.Draw();
	}

	auto matWorld = Matrix4::RotationY(0.0f);
	auto matScale = Matrix4::Scaling(1.0f);
	auto matWVP = Transpose(matScale * matWorld * matView * matProj);

	mDomeTexture.Bind();
	mShader.SetUniformMat4f("WVP", matWVP);
	mMeshBufferDome.Draw();
}