#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

namespace
{
	float numRows	 = 5;
	float numColumns = 5;
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
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Mirror);

	mPbrEffect.Initialize();
	mPbrEffect.SetDiffuseTexture("../../Assets/Images/PBR/RustedIron/rustediron2_basecolor.png");
	mPbrEffect.SetNormalTexture("../../Assets/Images/PBR/RustedIron/rustediron2_normal.png");
	mPbrEffect.SetMetallicTexture("../../Assets/Images/PBR/RustedIron/rustediron2_metallic.png");
	mPbrEffect.SetRoughnessTexture("../../Assets/Images/PBR/RustedIron/rustediron2_roughness.png");

	mSkybox.ChangeDefualtSkybox(1);
	mSkybox.CreateSkybox();

	mPlainTexture.Initialize("../../Assets/Images/white.jpg");
}

void GameState::Terminate()
{
	mPlainTexture.Terminate();
	mSkybox.Terminate();
	mSampler.Terminate();
	mPbrEffect.Terminate();

	mMeshBufferSphere.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 3.0f;
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

	mPbrEffect.Begin();
	mPbrEffect.SetDirectionalLight(mDirectionalLight);
	mPbrEffect.SetMaterial(mMaterial);

	if (useTextureMap)
	{
		// 1
		mPbrEffect.SetTransformData(matWorld, matView, matProj, mCamera.GetPosition());
		mPbrEffect.UpdateSettings();
		mMeshBufferTeaPot.Draw();

		// 2
		matTrans = Matrix4::Translation({ -4.0f,0.0f,0.0f });
		matWorld = matRot * matTrans;
		mPbrEffect.SetTransformData(matWorld, matView, matProj, mCamera.GetPosition());
		mPbrEffect.UpdateSettings();
		mMeshBufferSphere.Draw();

		// 3
		matTrans = Matrix4::Translation({ 4.0f,0.0f,0.0f });
		matWorld = matRot * matTrans;
		mPbrEffect.SetTransformData(matWorld, matView, matProj, mCamera.GetPosition());
		mPbrEffect.UpdateSettings();
		mMeshBufferCube.Draw();
	}
	else
	{
		mPbrEffect.SetDiffuseTexture(&mPlainTexture);
		float spacing = 1.3f;
		for (int row = 0; row < numRows; ++row)
		{
			mPbrEffect.SetMetallicWeight((float)row / (float)numRows);
			for (int col = 0; col < numColumns; ++col)
			{
				mPbrEffect.SetRoughnessWeight(Math::Clamp((float)col / (float)numColumns, 0.05f, 1.0f));
				matWorld = Matrix4::Translation({(col - (numColumns / 2)) * spacing, (row - (numRows / 2)) * spacing, 0.0f } );
				mPbrEffect.SetTransformData(matWorld, matView, matProj, mCamera.GetPosition());
				mPbrEffect.UpdateSettings();
				mMeshBufferSphere.Draw();
			}
		}
	}
	mPbrEffect.End();
	mSkybox.Draw(mCamera);
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
		ImGui::DragFloat("Power##Material", &mMaterial.power, 0.2f, 0.0f, 100.0f);
	}
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Rotation##Transform", &mRotation.x, 0.01f);
	}
	if (ImGui::CollapsingHeader("PBR Settings Map", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Checkbox("Show Texture Map", &useTextureMap))
		{
			if (useTextureMap)
			{
				mPbrEffect.SetRoughnessWeight(-1.0f);
				mPbrEffect.SetMetallicWeight(-1.0f);
				mPbrEffect.SetNormalMapWeight(1.0f);
			}
			else
			{
				mPbrEffect.SetNormalMapWeight(0.0f);
			}
		}
		//if (ImGui::SliderFloat("Rough", &brightness, 0.0f, 3.0f))
		//	mEnvironmentMap.SetBrightness(brightness);
		//if (ImGui::SliderFloat("Environment Ratio", &environmentRatio, 0.0f, 1.0f))
		//	mEnvironmentMap.SetEnvironmentRatio(environmentRatio);
		//if (ImGui::SliderFloat("Reflection to Refraction Ratio", &reflectRefractRatio, 0.0, 1.0f))
		//	mEnvironmentMap.SetReflectRefractRatio(reflectRefractRatio);
		//if (ImGui::SliderFloat("Refraction Index", &refractionIndex, -1.0f, 1.0f))
		//	mEnvironmentMap.SetRefractionIndex(refractionIndex);

	}
	ImGui::End();
}