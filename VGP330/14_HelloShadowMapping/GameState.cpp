#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{

	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mTankPosition = { 0.0f,3.5f,0.0f };

	mDefaultCamera.SetNearPlane(0.001f);
	mDefaultCamera.SetPosition({ 0.0f,5.0f,-20.0f });
	mDefaultCamera.SetDirection({ 0.0f,0.0f, 1.0f });

	mLightCamera.SetPosition(Normalize({ 1.0f, -1.0f, 1.0f }));
	mLightCamera.SetDirection(mTankPosition - mLightCamera.GetPosition() * 100.0f);
	mLightCamera.SetNearPlane(1.0f);
	mLightCamera.SetFarPlane(200.0f);
	mLightCamera.SetFov(1.0f);
	mLightCamera.SetAspectRatio(1.0f);

	mActiveCamera = &mDefaultCamera;
	////////////

	ObjLoader::Load("../../Assets/Models/Tank/tank.obj", 0.001f, mTankMesh);
	mTankMeshBuffer.Initialize(mTankMesh);

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f,1.0f });
	mDirectionalLight.ambient = { 0.0f,0.0f,0.0f ,1.0f };
	mDirectionalLight.diffuse = { 0.75f,0.75f,0.75f ,1.0f };
	mDirectionalLight.specular = { 0.5f,0.5f,0.5f ,1.0f };

	mMaterial.ambient = { 0.0f,0.0f,0.0f ,1.0f };
	mMaterial.diffuse = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.specular = { 0.5f,0.5f,0.5f ,1.0f };
	mMaterial.power = 80.0f;

	mVertexShader.Initialize("../../Assets/Shaders/Standard.fx", Vertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Standard.fx");

	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
	mTexture.Initialize("../../Assets/Models/Tank/tank_diffuse.jpg");
	mSpecularTexture.Initialize("../../Assets/Models/Tank/tank_specular.jpg");
	mNormalMap.Initialize("../../Assets/Models/Tank/tank_normal.jpg");

	mGroundTexture.Initialize("../../Assets/Images/grass.jpg");

	mBlendState.Initialize(BlendState::Mode::Additive);

	mGroundMesh = MeshBuilder::CreatePlane(20.0f);
	mGroundMeshBuffer.Initialize(mGroundMesh);

	//Quad
	auto graphicsSystem = GraphicsSystem::Get();
	mRenderTarget.Initialize(graphicsSystem->GetBackBufferWidth(), graphicsSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);
	mScreenQuad = MeshBuilder::CreateNDCQuad();
	mScreenQuadBuffer.Initialize(mScreenQuad);

	mPostProcessingVertexShader.Initialize("../../Assets/Shaders/PostProcessing.fx", VertexPX::Format);
	mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcessing.fx", "PSNoProcessing");

	SimpleDraw::AddGroundPlane(200.0f, true, Graphics::Colors::Blue);
}

void GameState::Terminate()
{
	mPostProcessingPixelShader.Terminate();
	mPostProcessingVertexShader.Terminate();
	mScreenQuadBuffer.Terminate();
	mRenderTarget.Terminate();
	//
	mGroundMeshBuffer.Terminate();

	mBlendState.Terminate();
	mGroundTexture.Terminate();
	mNormalMap.Terminate();
	mDisplacementTexture.Terminate();
	mTexture.Terminate();
	mSpecularTexture.Terminate();
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
	const float kMoveSpeed = 5.0f;
	const float kTurnSpeed = 1.0f;

	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
		mDefaultCamera.Walk(kMoveSpeed*deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mDefaultCamera.Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mDefaultCamera.Yaw(inputSystem->GetMouseMoveX() *kTurnSpeed*deltaTime);
		mDefaultCamera.Pitch(inputSystem->GetMouseMoveY() *kTurnSpeed*deltaTime);
	}

	if (inputSystem->IsKeyDown(KeyCode::A))
		mDefaultCamera.Strafe(-kMoveSpeed * deltaTime);
	//mTankPosition += deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::D))
		mDefaultCamera.Strafe(kMoveSpeed*deltaTime);
	//mTankPosition -= deltaTime;
}

void GameState::Render()
{
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
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool lightCamera = mActiveCamera == &mLightCamera;
		if (ImGui::Checkbox("Use Light Camera", &lightCamera))
		{
			mActiveCamera = lightCamera ? &mLightCamera : &mDefaultCamera;
		}
	}
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
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
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
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
		ImGui::SliderFloat("Displacement", &mSettings.bumpMapWeight, 0.0f, 1.0f);
		if (ImGui::Checkbox("Normal Map", &normal))
		{
			mSettings.normalMapWeight = normal ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Specular Map", &specular))
		{
			mSettings.specularMapWeight = specular ? 1.0f : 0.0f;
		}

	}
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Rotation##Transform", &mTankPosition.x, 0.01f);
	}
	ImGui::End();
}

void GameState::DrawScene()
{
	auto matTrans = Matrix4::Translation({ mTankPosition });
	auto matRot = Matrix4::RotationX(mTankPosition.x) * Matrix4::RotationY(mTankPosition.y) * Matrix4::RotationZ(mTankPosition.z);
	auto matWorld = matRot * matTrans;
	auto matView = mActiveCamera->GetViewMatrix();
	auto matProj = mActiveCamera->GetPerspectiveMatrix();
	TransformData transformData;

	mSampler.BindVS();
	mSampler.BindPS();

	mLightBuffer.Update(&mDirectionalLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(&mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.Update(&mSettings);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	//Tank
	mTexture.BindPS(0);
	mSpecularTexture.BindPS(1);
	mDisplacementTexture.BindVS(2);
	mNormalMap.BindPS(3);

	mTransformBuffer.BindVS(0);
	
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView *matProj);
	transformData.viewPosition = mActiveCamera->GetPosition();
	mTransformBuffer.Update(&transformData);

	mVertexShader.Bind();
	mPixelShader.Bind();

	mBlendState.ClearState();
	mTankMeshBuffer.Draw();

	//Ground
	matTrans = Matrix4::Translation({ 0.0f,0.0f,0.0f });
	matRot = Matrix4::RotationX(mTankPosition.x) * Matrix4::RotationY(mTankPosition.y) * Matrix4::RotationZ(mTankPosition.z);
	matWorld = matRot * matTrans;

	mTransformBuffer.BindVS(0);
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView *matProj);
	transformData.viewPosition = mActiveCamera->GetPosition();
	mTransformBuffer.Update(&transformData);

	mGroundTexture.BindPS(0);
	mVertexShader.Bind();
	mPixelShader.Bind();
	mGroundMeshBuffer.Draw();


	SimpleDraw::Render(*mActiveCamera);

}

void GameState::PostProcess()
{
	mPostProcessingPixelShader.Bind();
	mPostProcessingVertexShader.Bind();
	mSampler.BindPS();
	mScreenQuadBuffer.Draw();
}
