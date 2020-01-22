#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::LightGray);

	mCamera.SetPosition({ 0.0f, 0.0f,-2.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });

	mMesh = MeshBuilder::CreateSpherePN(1.0f,100,16);
	mMeshBuffer.Initialize(mMesh);

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f,1.0f });
	mDirectionalLight.ambient = { 0.3f };
	mDirectionalLight.diffuse = { 0.7f };
	mDirectionalLight.specular = { 0.5f };

	mMaterial.ambient = { 1.0f,0.0f,0.0f ,0.0f };
	mMaterial.diffuse = { 1.0f,0.0f,0.0f ,0.0f };
	mMaterial.specular = { 1.0f,0.0f,0.0f ,0.0f };
	mMaterial.power = 1.0f;

	mVertexShader.Initialize("../../Assets/Shaders/DoPhongLighting.fx", VertexPN::Format);
	mPixelShader.Initialize("../../Assets/Shaders/DoPhongLighting.fx");
}

void GameState::Terminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
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
		mCamera.Strafe(-kMoveSpeed * deltaTime);
	//mRotation += deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(kMoveSpeed*deltaTime);
	//mRotation -= deltaTime;

}

void GameState::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();

	auto matTrans = Matrix4::Translation({0.0f});
	auto matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y);
	auto matWorld = matRot * matTrans;
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView *matProj);
	transformData.viewPosition = mCamera.GetPosition();

	mTransformBuffer.Update(&transformData);
	mTransformBuffer.BindVS(0);

	mLightBuffer.Update(&mDirectionalLight);
	mLightBuffer.BindVS(1);

	mMaterialBuffer.Update(&mMaterial);
	mMaterialBuffer.BindVS(2);

	mVertexShader.Bind();
	mPixelShader.Bind();

	mMeshBuffer.Draw();

}

void GameState::DebugUI()
{
	ImGui::Begin("Setting",nullptr,ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Light"))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat("Direction X##Light", &mDirectionalLight.direction.x, 0.01f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Y##Light", &mDirectionalLight.direction.y, 0.01f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Z##Light", &mDirectionalLight.direction.z, 0.01f, -1.0f, 1.0f);
		if (directionChanged)
		{
			mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.x);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.x);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.x);
	}
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.x);
		ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.x);
		ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.x);
		ImGui::DragFloat("Power##Material", &mMaterial.power, 1.0f, 1.0f, 200.0f);
	}
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::DragFloat3("Rotation##Transform", &mRotation.x, 0.01f);
	}
	ImGui::End();
}