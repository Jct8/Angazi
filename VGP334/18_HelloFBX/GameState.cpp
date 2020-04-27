#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ 0.0f,0.0f,-40.0f });
	mCamera.SetDirection({ 0.0f,0.0f, 1.0f });

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();

	mDirectionalLight.direction = Normalize({ -0.914f, 0.261f, 0.309f });
	mDirectionalLight.direction = Normalize({ 0.985f,-0.069f, 0.156f });
	mDirectionalLight.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mDirectionalLight.diffuse = { 0.75f,0.75f,0.75f ,1.0f };
	mDirectionalLight.specular = { 0.5f,0.5f,0.5f ,1.0f };

	mMaterial.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.diffuse = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.specular = { 0.5f,0.5f,0.5f ,1.0f };
	mMaterial.power = 80.0f;

	mVertexShader.Initialize("../../Assets/Shaders/Standard.fx", Vertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Standard.fx");

	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
	mBlendState.Initialize(BlendState::Mode::Additive);

	// Post Processing
	auto graphicsSystem = GraphicsSystem::Get();
	mRenderTarget.Initialize(graphicsSystem->GetBackBufferWidth(), graphicsSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);
	mScreenQuad = MeshBuilder::CreateNDCQuad();
	mScreenQuadBuffer.Initialize(mScreenQuad);
	mPostProcessingVertexShader.Initialize("../../Assets/Shaders/PostProcessing.fx", VertexPX::Format);
	mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcessing.fx", "PSNoProcessing");

	// Jet
	mJetPosition = { 0.0f,0.0f,0.0f };
	ObjLoader::Load("../../Assets/Models/Jet/F 15.obj", 1.0f, mJetMesh);
	mJetMeshBuffer.Initialize(mJetMesh);
	mJetTexture.Initialize("../../Assets/Models/Jet/F 15E.jpg");
	mJetSpecularTexture.Initialize("../../Assets/Models/Jet/F 15 Specular.jpg");
	mJetNormalMap.Initialize("../../Assets/Models/Jet/F-15C normal.jpg");
	mJetAOMap.Initialize("../../Assets/Models/Jet/F-15CAO.jpg");

	// Settings
	mSettings.brightness = 0.825f;
	mSettings.bumpMapWeight = 0.165f;

	mAnimation = Graphics::AnimationBuilder()
		.SetTime(0.0f)
			.AddPositionKey(Math::Vector3(-20.0f,0.0f,0.0f))
			.AddRotationKey(Math::Quaternion())
			.AddScaleKey(Math::Vector3::One)
		.SetTime(5.0f)
			.AddPositionKey(Math::Vector3(0.0f, 20.0f, 20.0f))
			.AddRotationKey(Math::Quaternion::RotationAxis(Math::Vector3(-1.0f,1.0f,-1.0f), Math::Constants::DegToRad * 40.0f))
		.SetTime(10.0f)
			.AddPositionKey(Math::Vector3(20.0f, 0.0f, 0.0f))
			.AddRotationKey(Math::Quaternion::RotationAxis(Math::Vector3(0.0f, 1.0f, 0.0f), Math::Constants::DegToRad * 180.0f))
		.SetTime(15.0f)
			.AddPositionKey(Math::Vector3(0.0f, -20.0f, -20.0f))
			.AddRotationKey(Math::Quaternion::RotationAxis(Math::Vector3(-1.0f, 1.0f, -1.0f), Math::Constants::DegToRad * 270.0f))
		.SetTime(20.0f)
			.AddPositionKey(Math::Vector3(-20.0f, 0.0f, 0.0f))
			.AddRotationKey(Math::Quaternion())
		.SetLooping(true)
		.Build();

	model.Initialize("../../Assets/Models/character.model");
}

void GameState::Terminate()
{
	// Model
	model.Terminate();

	//Jet
	mJetAOMap.Terminate();
	mJetNormalMap.Terminate();
	mJetDisplacementTexture.Terminate();
	mJetTexture.Terminate();
	mJetMeshBuffer.Terminate();

	mPostProcessingPixelShader.Terminate();
	mPostProcessingVertexShader.Terminate();
	mScreenQuadBuffer.Terminate();
	mRenderTarget.Terminate();
	//
	mBlendState.Terminate();
	mSampler.Terminate();

	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 10.0f;
	const float kTurnSpeed = 1.0f;

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
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(kMoveSpeed*deltaTime);

	dt += deltaTime;
}

void GameState::Render()
{
	//mRenderTarget.BeginRender();
	DrawScene();
	//mRenderTarget.EndRender();

	//mRenderTarget.BindPS(0);
	//PostProcess();
	//mRenderTarget.UnbindPS(0);
}

void GameState::DebugUI()
{
	ImGui::Begin("Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Light"))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat("Direction X##Light", &mDirectionalLight.direction.x, 0.01f, -2.0f, 2.0f);
		directionChanged |= ImGui::DragFloat("Direction Y##Light", &mDirectionalLight.direction.y, 0.01f, -2.0f, 2.0f);
		directionChanged |= ImGui::DragFloat("Direction Z##Light", &mDirectionalLight.direction.z, 0.01f, -2.0f, 2.0f);
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
		ImGui::DragFloat("Power##Material", &mMaterial.power, 1.0f, 1.0f, 100.0f);
	}
	if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		static bool normal = true;
		static bool specular = true;
		static bool aoMap = true;
		static bool useShadow = mSettings.useShadow == 1;
		ImGui::SliderFloat("Displacement", &mSettings.bumpMapWeight, 0.0f, 1.0f);
		if (ImGui::Checkbox("Normal Map", &normal))
		{
			mSettings.normalMapWeight = normal ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Specular Map", &specular))
		{
			mSettings.specularMapWeight = specular ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Ambient occlusion", &aoMap))
		{
			mSettings.aoMapWeight = aoMap ? 1.0f : 0.0f;
		}
		ImGui::SliderFloat("Brightness", &mSettings.brightness, 0.0f, 10.f);
	}
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("JetTranslation##Transform", &mJetPosition.x, 0.3f);
	}
	ImGui::End();
}

void GameState::DrawScene()
{
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	mSampler.BindVS();
	mSampler.BindPS();

	TransformData transformData;
	mTransformBuffer.BindVS(0);

	mLightBuffer.Update(&mDirectionalLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(&mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	// Jet
	mSettingsBuffer.Update(&mSettings);

	mJetTexture.BindPS(0);
	mJetSpecularTexture.BindPS(1);
	mJetDisplacementTexture.BindVS(2);
	mJetNormalMap.BindPS(3);
	mJetAOMap.BindPS(4);

	mVertexShader.Bind();
	mPixelShader.Bind();

	auto matWorld = mAnimation.GetTransform(dt);

	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView *matProj);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Update(&transformData);

	mJetMeshBuffer.Draw();

	auto matRot = Matrix4::RotationY(Constants::Pi);
	matWorld = Matrix4::Scaling(0.1f) * matRot * matWorld;

	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView *matProj);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Update(&transformData);

	model.Draw();
}

void GameState::PostProcess()
{
	mPostProcessingPixelShader.Bind();
	mPostProcessingVertexShader.Bind();
	mSampler.BindPS();
	mScreenQuadBuffer.Draw();
}
