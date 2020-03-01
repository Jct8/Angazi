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

	mLightCamera.SetPosition(Normalize(Vector3{ -1.0f, 1.0f, -1.0f }) * 50);
	mLightCamera.SetDirection(mTankPosition - mLightCamera.GetPosition() );
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
	mAOMap.Initialize("../../Assets/Models/Tank/tank_ao.jpg");

	mGroundTexture.Initialize("../../Assets/Images/grass.jpg");

	mBlendState.Initialize(BlendState::Mode::Additive);

	mGroundMesh = MeshBuilder::CreatePlane(200.0f, 100, 100);
	mGroundMeshBuffer.Initialize(mGroundMesh);

	//Quad
	auto graphicsSystem = GraphicsSystem::Get();

	constexpr uint32_t depthMapSize = 1024;
	mDepthMapRenderTarget.Initialize(depthMapSize, depthMapSize, RenderTarget::Format::RGBA_U32);
	mDepthMapVertexShader.Initialize("../../Assets/Shaders/DepthMap.fx", Vertex::Format);
	mDepthMapPixelShader.Initialize("../../Assets/Shaders/DepthMap.fx");
	mDepthMapConstantBuffer.Initialize();

	mRenderTarget.Initialize(graphicsSystem->GetBackBufferWidth(), graphicsSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);
	mScreenQuad = MeshBuilder::CreateNDCQuad();
	mScreenQuadBuffer.Initialize(mScreenQuad);

	mPostProcessingVertexShader.Initialize("../../Assets/Shaders/PostProcessing.fx", VertexPX::Format);
	mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcessing.fx", "PSNoProcessing");

	SimpleDraw::AddGroundPlane(200.0f, true, Graphics::Colors::Blue);

	mShadowConstantBuffer.Initialize();

}

void GameState::Terminate()
{
	mShadowConstantBuffer.Terminate();

	mPostProcessingPixelShader.Terminate();
	mPostProcessingVertexShader.Terminate();
	mScreenQuadBuffer.Terminate();
	mRenderTarget.Terminate();

	mDepthMapRenderTarget.Terminate();
	mDepthMapVertexShader.Terminate();
	mDepthMapPixelShader.Terminate();
	mDepthMapConstantBuffer.Terminate();

	//
	mGroundMeshBuffer.Terminate();

	mBlendState.Terminate();
	mGroundTexture.Terminate();
	mAOMap.Terminate();
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
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mDefaultCamera.Yaw(inputSystem->GetMouseMoveX() *kTurnSpeed*deltaTime);
		mDefaultCamera.Pitch(inputSystem->GetMouseMoveY() *kTurnSpeed*deltaTime);
		if (inputSystem->IsKeyDown(KeyCode::W))
		{
			mDefaultCamera.Walk(kMoveSpeed*deltaTime);
		}
		if (inputSystem->IsKeyDown(KeyCode::S))
		{
			mDefaultCamera.Walk(-kMoveSpeed * deltaTime);
		}
		if (inputSystem->IsKeyDown(KeyCode::A))
		{
			mDefaultCamera.Strafe(-kMoveSpeed * deltaTime);
		}
		if (inputSystem->IsKeyDown(KeyCode::D))
		{
			mDefaultCamera.Strafe(kMoveSpeed*deltaTime);
		}
		return;
	}
	if (inputSystem->IsKeyDown(KeyCode::W))
	{
		mTankPosition.z += kMoveSpeed * deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::S))
	{
		mTankPosition.z -= kMoveSpeed * deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mTankPosition.x -= kMoveSpeed * deltaTime;
		mTankRotation.y -= kMoveSpeed * deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mTankPosition.x += kMoveSpeed * deltaTime;
		mTankRotation.y += kMoveSpeed * deltaTime;
	}
	//mLightCamera.SetDirection(mTankPosition - mLightCamera.GetPosition());
	mLightCamera.SetDirection(mDirectionalLight.direction);

	//mTankPosition -= deltaTime;
}

void GameState::Render()
{
	mDepthMapRenderTarget.BeginRender();
	DrawDepthMap();
	mDepthMapRenderTarget.EndRender();

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

		ImGui::Image(
			mDepthMapRenderTarget.GetShaderResourceView(),
			{ 150.0f,150.0f },
			{ 0.0f,0.0f },
			{ 1.0f,1.0f },
			{ 1.0f,1.0f ,1.0f,1.0f },
			{ 1.0f,1.0f ,1.0f,1.0f}
		);
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
		if (ImGui::Checkbox("Ambient occlusion", &aoMap))
		{
			mSettings.aoMapWeight = aoMap ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Use Shadow", &useShadow))
		{
			mSettings.useShadow = useShadow ? 1 : 0;
		}
		ImGui::SliderFloat("Brightness", &mSettings.brightness, 1.0f, 10.f);
		ImGui::SliderFloat("Depth Bias", &mSettings.depthBias, 0.0001f, 0.0009f);
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
	auto matRot = Matrix4::RotationX(mTankRotation.x) * Matrix4::RotationY(mTankRotation.y) * Matrix4::RotationZ(mTankRotation.z);
	auto matWorld = matRot * matTrans;

	auto matView = mActiveCamera->GetViewMatrix();
	auto matProj = mActiveCamera->GetPerspectiveMatrix();
	auto matViewLight = mLightCamera.GetViewMatrix();
	auto matProjLight = mLightCamera.GetPerspectiveMatrix();
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

	auto wvpLight = Transpose(matWorld * matViewLight * matProjLight);
	mShadowConstantBuffer.Update(&wvpLight);
	mShadowConstantBuffer.BindVS(4);

	//Tank
	mTexture.BindPS(0);
	mSpecularTexture.BindPS(1);
	mDisplacementTexture.BindVS(2);
	mNormalMap.BindPS(3);
	mAOMap.BindPS(4);
	mDepthMapRenderTarget.BindPS(5);

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
	matRot = Matrix4::Identity;// Matrix4::RotationX(mTankPosition.x) * Matrix4::RotationY(mTankPosition.y) * Matrix4::RotationZ(mTankPosition.z);
	matWorld = matRot * matTrans;

	wvpLight = Transpose(matWorld * matViewLight * matProjLight);
	mShadowConstantBuffer.Update(&wvpLight);
	mShadowConstantBuffer.BindVS(4);

	mTransformBuffer.BindVS(0);
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView *matProj);
	transformData.viewPosition = mActiveCamera->GetPosition();
	mTransformBuffer.Update(&transformData);

	mGroundSettings.aoMapWeight = 0.0f;
	mGroundSettings.normalMapWeight = 0.0f;
	mGroundSettings.bumpMapWeight = 0.0f;
	mGroundSettings.specularMapWeight = 0.0f;
	mGroundSettings.brightness = 1.0f;
	mGroundSettings.useShadow = 1;

	mSettingsBuffer.Update(&mGroundSettings);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mGroundTexture.BindPS(0);
	mVertexShader.Bind();
	mPixelShader.Bind();
	mGroundMeshBuffer.Draw();


	SimpleDraw::Render(*mActiveCamera);

}

void GameState::DrawDepthMap()
{
	mDepthMapPixelShader.Bind();
	mDepthMapVertexShader.Bind();

	auto matViewLight = mLightCamera.GetViewMatrix();
	auto matProjLight = mLightCamera.GetPerspectiveMatrix();

	auto matTrans = Matrix4::Translation({ mTankPosition });
	auto matRot = Matrix4::RotationX(mTankRotation.x) * Matrix4::RotationY(mTankRotation.y) ;
	auto matWorld = matRot * matTrans;

	auto wvp = Transpose(matWorld * matViewLight * matProjLight);
	mDepthMapConstantBuffer.Update(&wvp);
	mDepthMapConstantBuffer.BindVS(0);

	mTankMeshBuffer.Draw();
}

void GameState::PostProcess()
{
	mPostProcessingPixelShader.Bind();
	mPostProcessingVertexShader.Bind();
	mSampler.BindPS();
	mScreenQuadBuffer.Draw();
}
