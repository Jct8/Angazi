#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ 0.0f,10.0f,-20.0f });
	mCamera.SetDirection({ 0.0f,-1.0f, 1.0f });

	////////////
	mMesh = MeshBuilder::CreatePlane(200.0f, 20, 20);
	mMeshBuffer.Initialize(mMesh);

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
	mGroundTexture.Initialize("../../Assets/Images/grass.jpg");

	mBlendState.Initialize(BlendState::Mode::Additive);

	// Post Processing
	auto graphicsSystem = GraphicsSystem::Get();
	mRenderTarget.Initialize(graphicsSystem->GetBackBufferWidth(), graphicsSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);
	mScreenQuad = MeshBuilder::CreateNDCQuad();
	mScreenQuadBuffer.Initialize(mScreenQuad);

	mPostProcessingVertexShader.Initialize("../../Assets/Shaders/PostProcessing.fx", VertexPX::Format);
	mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcessing.fx", "PSNoProcessing");

	// Tank
	mTankPosition = { 0.0f,0.0f,0.0f };
	ObjLoader::Load("../../Assets/Models/Tank/tank.obj", 0.001f, mTankMesh);
	mTankMeshBuffer.Initialize(mTankMesh);
	mTankTexture.Initialize("../../Assets/Models/Tank/tank_diffuse.jpg");
	mTankSpecularTexture.Initialize("../../Assets/Models/Tank/tank_specular.jpg");
	mTankNormalMap.Initialize("../../Assets/Models/Tank/tank_normal.jpg");
	mTankAOMap.Initialize("../../Assets/Models/Tank/tank_ao.jpg");

	// Settings
	mGroundSettings.bumpMapWeight = 0.0f;
	mGroundSettings.brightness = 1.0f;
	mGroundSettings.normalMapWeight = 0.0f;
	mTankSettings.bumpMapWeight = 0.0f;
	mTankSettings.brightness = 10.0f;
	mSettings.brightness = 1.7f;
	mSettings.bumpMapWeight = 0.165f;
	mSettings.movementSpeed = 0.020f;

	mAnimation = Graphics::AnimationBuilder()
		.SetTime(0.0f)
			.AddPositionKey(Math::Vector3(-50.0f,0.0f,0.0f))
			.AddRotationKey(Math::Quaternion())
			.AddScaleKey(Math::Vector3::One)
		.SetTime(5.0f)
			.AddPositionKey(Math::Vector3(0.0f, 20.0f, 30.0f))
			.AddRotationKey(Math::Quaternion::RotationAxis(Math::Vector3(-1.0f,1.0f,-1.0f), Math::Constants::DegToRad * 40.0f))
		.SetTime(10.0f)
			.AddPositionKey(Math::Vector3(50.0f, 0.0f, 0.0f))
			.AddRotationKey(Math::Quaternion::RotationAxis(Math::Vector3(0.0f, 1.0f, 0.0f), Math::Constants::DegToRad * 90.0f))
		.SetTime(15.0f)
			.AddPositionKey(Math::Vector3(0.0f, -20.0f, -30.0f))
			.AddRotationKey(Math::Quaternion::RotationAxis(Math::Vector3(-1.0f, 1.0f, -1.0f), Math::Constants::DegToRad * 120.0f))
		.SetTime(20.0f)
			.AddPositionKey(Math::Vector3(-50.0f, 0.0f, 0.0f))
			.AddRotationKey(Math::Quaternion())
		.SetLooping(true)
		.Build();
}

void GameState::Terminate()
{
	//Tank
	mTankAOMap.Terminate();
	mTankNormalMap.Terminate();
	mTankDisplacementTexture.Terminate();
	mTankTexture.Terminate();
	mTankMeshBuffer.Terminate();

	mPostProcessingPixelShader.Terminate();
	mPostProcessingVertexShader.Terminate();
	mScreenQuadBuffer.Terminate();
	mRenderTarget.Terminate();
	//
	mBlendState.Terminate();
	mGroundTexture.Terminate();
	mSampler.Terminate();

	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
	mMeshBuffer.Terminate();
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

	mSettings.movement += mSettings.movementSpeed * deltaTime;
	if (mSettings.movement >= 1.0f)
		mSettings.movement -= 1.0f;

	dt += deltaTime;
}

void GameState::Render()
{
	/////Normal///////
	mRenderTarget.BeginRender();
	DrawScene();
	mRenderTarget.EndRender();

	mRenderTarget.BindPS(0);
	PostProcess();
	mRenderTarget.UnbindPS(0);
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
		if (ImGui::Checkbox("Use Shadow", &useShadow))
		{
			mSettings.useShadow = useShadow ? 1 : 0;
		}
		ImGui::SliderFloat("Brightness", &mSettings.brightness, 0.0f, 10.f);
		ImGui::SliderFloat("Movement Speed", &mSettings.movementSpeed, 0.0001f, 0.1f);
	}
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("TankTranslation##Transform", &mTankPosition.x, 0.3f);
		ImGui::DragFloat3("GroundTranslation##Transform", &mGrounddTranslation.x, 0.3f);
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

	//Tank
	mSettingsBuffer.Update(&mTankSettings);

	mTankTexture.BindPS(0);
	mTankSpecularTexture.BindPS(1);
	mTankDisplacementTexture.BindVS(2);
	mTankNormalMap.BindPS(3);
	mTankAOMap.BindPS(4);

	mVertexShader.Bind();
	mPixelShader.Bind();

	auto matTrans = Matrix4::Translation({ mTankPosition });
	auto matRot = Matrix4::RotationX(mTankRotation.x) * Matrix4::RotationY(mTankRotation.y) * Matrix4::RotationZ(mTankRotation.z);
	auto matWorld = Matrix4::Scaling(0.5f)* matRot * matTrans;

	matWorld = mAnimation.GetTransform(dt);

	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView *matProj);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Update(&transformData);

	mTankMeshBuffer.Draw();

	//ground
	mGroundSettings.bumpMapWeight = 0.0f;
	mGroundSettings.brightness = 1.0f;
	mGroundSettings.aoMapWeight = 0.0f;
	mSettingsBuffer.Update(&mGroundSettings);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mVertexShader.Bind();
	mPixelShader.Bind();

	matTrans = Matrix4::Translation({ mGrounddTranslation });
	matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z);
	matWorld = matRot * matTrans;

	mGroundTexture.BindPS(0);

	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView *matProj);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Update(&transformData);

	mMeshBuffer.Draw();

}

void GameState::PostProcess()
{
	mPostProcessingPixelShader.Bind();
	mPostProcessingVertexShader.Bind();
	mSampler.BindPS();
	mScreenQuadBuffer.Draw();
}
