#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

namespace
{
	float brightness = 1.0f;
	float environmentRatio = 0.6f;
	float reflectRefractRatio = 0.5f;
	float refractionIndex = 1.0f / 1.33f;
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ 0.0f, 0.0f,-4.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f,1.0f });
	mDirectionalLight.ambient = { 0.3f };
	mDirectionalLight.diffuse = { 0.7f };
	mDirectionalLight.specular = { 0.5f };

	mMaterial.ambient = { 0.3f };
	mMaterial.diffuse = { 0.7f };
	mMaterial.specular = { 0.5f };
	mMaterial.power = 80.0f;

	mMeshBufferSphere.Initialize(MeshBuilder::CreateSphere(0.5f, 32, 32));
	ObjLoader::Load("../../Assets/Models/Teapot/utah-teapot.obj", 0.05f, mMeshTeapot);
	mMeshBufferTeaPot.Initialize(mMeshTeapot);
	mMeshBufferCube.Initialize(MeshBuilder::CreateCube());

	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);

	mEnvironmentMap.Initialize();
	mEnvironmentMap.SetDiffuseTexture("../../Assets/Images/white.jpg");

	mSkybox.ChangeDefualtSkybox(1);
	mSkybox.CreateSkybox();
	mHdrEffect.Initialize();
}

void GameState::Terminate()
{
	mHdrEffect.Terminate();
	mSkybox.Terminate();
	mSampler.Terminate();

	mEnvironmentMap.Terminate();

	mMeshBufferCube.Terminate();
	mMeshBufferTeaPot.Terminate();
	mMeshBufferSphere.Terminate();
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
	auto matTrans = Matrix4::Translation({ 0.0f,0.0f,0.0f });
	auto matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z);
	auto matWorld = matRot * matTrans;
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	mHdrEffect.BeginRender();

	mEnvironmentMap.Begin();
	mEnvironmentMap.SetDirectionalLight(mDirectionalLight);
	mEnvironmentMap.SetMaterial(mMaterial);
	mEnvironmentMap.SetCubeMapTexture(mSkybox.GetSkyboxTexture());

	// 1
	mEnvironmentMap.SetTransformData(matWorld,matView,matProj,mCamera.GetPosition());
	mEnvironmentMap.UpdateSettings();
	mMeshBufferTeaPot.Draw();

	// 2
	matTrans = Matrix4::Translation({ -4.0f,0.0f,0.0f });
	matWorld = matRot * matTrans;
	mEnvironmentMap.SetTransformData(matWorld, matView, matProj, mCamera.GetPosition());
	mEnvironmentMap.UpdateSettings();
	mMeshBufferSphere.Draw();

	// 3
	matTrans = Matrix4::Translation({ 4.0f,0.0f,0.0f });
	matWorld = matRot * matTrans;
	mEnvironmentMap.SetTransformData(matWorld, matView, matProj, mCamera.GetPosition());
	mEnvironmentMap.UpdateSettings();
	mMeshBufferCube.Draw();

	mEnvironmentMap.End();
	mSkybox.Draw(mCamera);

	mHdrEffect.EndRender();
	mHdrEffect.RenderHdrQuad();
}

void GameState::DebugUI()
{
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %.2f", Angazi::Core::TimeUtil::GetFramesPerSecond());
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
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
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.x);
		ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.x);
		ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.x);
		ImGui::DragFloat("Power##Material", &mMaterial.power, 1.0f, 1.0f, 100.0f);
	}
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Rotation##Transform", &mRotation.x, 0.01f);
	}
	if (ImGui::CollapsingHeader("Environment Map", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 3.0f))
			mEnvironmentMap.SetBrightness(brightness);
		if (ImGui::SliderFloat("Environment Ratio", &environmentRatio, 0.0f, 1.0f))
			mEnvironmentMap.SetEnvironmentRatio(environmentRatio);
		if (ImGui::SliderFloat("Reflection to Refraction Ratio", &reflectRefractRatio, 0.0, 1.0f))
			mEnvironmentMap.SetReflectRefractRatio(reflectRefractRatio);
		if (ImGui::SliderFloat("Refraction Index", &refractionIndex, -1.0f, 1.0f))
			mEnvironmentMap.SetRefractionIndex(refractionIndex);

	}
	ImGui::End();
}