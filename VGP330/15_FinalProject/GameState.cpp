#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{

	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ 0.0f,3.0f,-5.0f });
	mCamera.SetDirection({ 0.0f,0.0f, 1.0f });

	////////////
	mMesh = MeshBuilder::CreatePlane(200.0f, 20, 20);
	mMeshBuffer.Initialize(mMesh);

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();

	mDirectionalLight.direction = Normalize({ -0.914f, 0.261f, 0.309f });
	mDirectionalLight.direction = Normalize({ 0.985f,- 0.069f, 0.156f });
	mDirectionalLight.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mDirectionalLight.diffuse = { 0.75f,0.75f,0.75f ,1.0f };
	mDirectionalLight.specular = { 0.5f,0.5f,0.5f ,1.0f };

	mMaterial.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.diffuse = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.specular = { 0.5f,0.5f,0.5f ,1.0f };
	mMaterial.power = 80.0f;

	mVertexShader.Initialize("../../Assets/Shaders/Water.fx", Vertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Water.fx");

	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
	mTexture.Initialize("../../Assets/Images/water2.jpg");
	mGroundTexture.Initialize("../../Assets/Images/grass.jpg");
	mSpecularTexture.Initialize("../../Assets/Images/waterSpec.jpg");
	mDisplacementTexture.Initialize("../../Assets/Images/dudv2.png");
	mNormalMap.Initialize("../../Assets/Images/waterNormal.png");

	mBlendState.Initialize(BlendState::Mode::Additive);

	// Post Processing
	auto graphicsSystem = GraphicsSystem::Get();
	mRenderTarget.Initialize(graphicsSystem->GetBackBufferWidth(), graphicsSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);
	mScreenQuad = MeshBuilder::CreateNDCQuad();
	mScreenQuadBuffer.Initialize(mScreenQuad);

	mPostProcessingVertexShader.Initialize("../../Assets/Shaders/PostProcessing.fx", VertexPX::Format);
	mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcessing.fx", "PSNoProcessing");

	// Refraction
	mRefractionRenderTarget.Initialize(graphicsSystem->GetBackBufferWidth(), graphicsSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U32);
	mRefractionVertexShader.Initialize("../../Assets/Shaders/StandardClipping.fx", Vertex::Format);
	mRefractionPixelShader.Initialize("../../Assets/Shaders/StandardClipping.fx");
	mRefractionConstantBuffer.Initialize();

	mShadowConstantBuffer.Initialize();

	// Clipping
	mClippingConstantBuffer.Initialize();

	// Tank
	mTankPosition = { 0.0f,3.5f,0.0f };
	ObjLoader::Load("../../Assets/Models/Tank/tank.obj", 0.001f, mTankMesh);
	mTankMeshBuffer.Initialize(mTankMesh);
	mTankTexture.Initialize("../../Assets/Models/Tank/tank_diffuse.jpg");
	mTankSpecularTexture.Initialize("../../Assets/Models/Tank/tank_specular.jpg");
	mTankNormalMap.Initialize("../../Assets/Models/Tank/tank_normal.jpg");
	mTankAOMap.Initialize("../../Assets/Models/Tank/tank_ao.jpg");

	// Reflections
	mReflectionRenderTarget.Initialize(graphicsSystem->GetBackBufferWidth(), graphicsSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U32);

	// Terrain
	mTerrain.Initialize(200, 200, 1.0f);
	mTerrain.SetHeightScale(30.0f);
	mTerrain.LoadHeightmap("../../Assets/Heightmaps/heightmap_200x200.raw");
	
	// Settings
	mGroundSettings.bumpMapWeight = 0.0f;
	mGroundSettings.brightness = 1.0f;
	mGroundSettings.normalMapWeight = 0.0f;
	mTankSettings.bumpMapWeight = 0.0f;
	mTankSettings.brightness = 10.0f;
	mSettings.brightness = 1.7;
	mSettings.bumpMapWeight = 0.165f;
	mSettings.movementSpeed = 0.020f;

	mTranslation = { 21.3f,2.1f,43.5f };
	mTankPosition = { 18.0f,3.8f,40.2f };

}

void GameState::Terminate()
{
	mTerrain.Terminate();

	mReflectionRenderTarget.Terminate();

	//Tank
	mTankAOMap.Terminate();
	mTankNormalMap.Terminate();
	mTankDisplacementTexture.Terminate();
	mTankTexture.Terminate();
	mTankMeshBuffer.Terminate();

	mClippingConstantBuffer.Terminate();

	mShadowConstantBuffer.Terminate();
	mRefractionRenderTarget.Terminate();
	mRefractionVertexShader.Terminate();
	mRefractionPixelShader.Terminate();
	mRefractionConstantBuffer.Terminate();

	mPostProcessingPixelShader.Terminate();
	mPostProcessingVertexShader.Terminate();
	mScreenQuadBuffer.Terminate();
	mRenderTarget.Terminate();
	//
	mBlendState.Terminate();
	mNormalMap.Terminate();
	mDisplacementTexture.Terminate();
	mGroundTexture.Terminate();
	mTexture.Terminate();
	mSpecularTexture.Terminate();
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

}

void GameState::Render()
{
	/////Refraction///////
	mRefractionRenderTarget.BeginRender();
	DrawScene(RenderType::Refraction);
	mRefractionRenderTarget.EndRender();


	/////Reflection///////
	auto cameraPosition = mCamera.GetPosition();
	auto cameradirection = mCamera.GetDirection();
	float distance = 2 * (cameraPosition.y - mTranslation.y);
	mCamera.SetPosition({ cameraPosition.x,cameraPosition.y - distance,cameraPosition.z });
	mCamera.SetDirection({ cameradirection.x,-cameradirection.y,cameradirection.z });

	mReflectionRenderTarget.BeginRender();
	DrawScene(RenderType::Reflection);
	mReflectionRenderTarget.EndRender();

	mCamera.SetPosition(cameraPosition);
	mCamera.SetDirection(cameradirection);

	/////Normal///////
	mRenderTarget.BeginRender();
	DrawScene(RenderType::Normal);
	mRenderTarget.EndRender();

	mRenderTarget.BindPS(0);
	PostProcess();
	mRenderTarget.UnbindPS(0);
}

void GameState::DebugUI()
{
	ImGui::Begin("Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Refraction Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Image(
			mRefractionRenderTarget.GetShaderResourceView(),
			{ 150.0f,150.0f },
			{ 0.0f,0.0f },
			{ 1.0f,1.0f },
			{ 1.0f,1.0f ,1.0f,1.0f },
			{ 1.0f,1.0f ,1.0f,1.0f }
		);
	}
	if (ImGui::CollapsingHeader("Reflection Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Image(
			mReflectionRenderTarget.GetShaderResourceView(),
			{ 150.0f,150.0f },
			{ 0.0f,0.0f },
			{ 1.0f,1.0f },
			{ 1.0f,1.0f ,1.0f,1.0f },
			{ 1.0f,1.0f ,1.0f,1.0f }
		);
	}
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
		ImGui::DragFloat3("Rotation##Transform", &mRotation.x, 0.01f);
		ImGui::DragFloat3("WaterTranslation##Transform", &mTranslation.x, 0.3f);
		ImGui::DragFloat3("TankTranslation##Transform", &mTankPosition.x, 0.3f);
		ImGui::DragFloat3("GroundTranslation##Transform", &mGrounddTranslation.x, 0.3f);
	}
	ImGui::End();
}

void GameState::DrawScene(RenderType rendertype)
{
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	mSampler.BindVS();
	mSampler.BindPS();

	switch (rendertype)
	{
	case GameState::Reflection:
		mClipping.plane = { 0.0f, 1.0f,0.0f, -mTranslation.y }; // Cull Down
		break;
	case GameState::Refraction:
		mClipping.plane = { 0.0f, -1.0f,0.0f, mTranslation.y }; // Cull Up
		break;
	default:
		mClipping.plane = { 0.0f, 0.0f,0.0f,0.0f }; // No Cull
		break;
	}
	mTerrain.SetClippingPlane(mClipping.plane);
	mClippingConstantBuffer.Update(&mClipping);
	mClippingConstantBuffer.BindVS(5);

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

	auto matTrans = Matrix4::Translation(mTranslation);
	auto matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z);
	auto matWorld = matRot * matTrans;

	//Water
	if (rendertype == RenderType::Normal)
	{
		mSettingsBuffer.Update(&mSettings);

		matTrans = Matrix4::Translation({ mTranslation });
		matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z);
		matWorld = matRot * matTrans;

		mTexture.BindPS(0);
		mRefractionRenderTarget.BindPS(6);
		mReflectionRenderTarget.BindPS(7);
		mSpecularTexture.BindPS(1);
		mDisplacementTexture.BindVS(2);
		mDisplacementTexture.BindPS(2);
		mNormalMap.BindPS(3);

		transformData.world = Transpose(matWorld);
		transformData.wvp = Transpose(matWorld * matView *matProj);
		transformData.viewPosition = mCamera.GetPosition();
		mTransformBuffer.Update(&transformData);

		mVertexShader.Bind();
		mPixelShader.Bind();

		//mBlendState.Bind();
		mMeshBuffer.Draw();
		mBlendState.ClearState();
	}

	//Tank
	mSettingsBuffer.Update(&mTankSettings);

	mTankTexture.BindPS(0);
	mTankSpecularTexture.BindPS(1);
	mTankDisplacementTexture.BindVS(2);
	mTankNormalMap.BindPS(3);
	mTankAOMap.BindPS(4);

	mRefractionVertexShader.Bind();
	mRefractionPixelShader.Bind();

	matTrans = Matrix4::Translation({ mTankPosition });
	matRot = Matrix4::RotationX(mTankRotation.x) * Matrix4::RotationY(mTankRotation.y) * Matrix4::RotationZ(mTankRotation.z);
	matWorld = Matrix4::Scaling(0.5f)* matRot * matTrans;

	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView *matProj);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Update(&transformData);

	mTankMeshBuffer.Draw();



	//ground
	mGroundSettings.bumpMapWeight = 0.0f;
	mGroundSettings.brightness = 1.0f;
	mSettingsBuffer.Update(&mGroundSettings);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mRefractionPixelShader.Bind();
	mRefractionVertexShader.Bind();
	matTrans = Matrix4::Translation({ mGrounddTranslation });
	matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z);
	matWorld =  matRot * matTrans;

	mGroundTexture.BindPS(0);

	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView *matProj);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Update(&transformData);

	mMeshBuffer.Draw();

	mTerrain.SetDirectionalLight(mDirectionalLight);
	mTerrain.Render(mCamera);
}

void GameState::PostProcess()
{
	mPostProcessingPixelShader.Bind();
	mPostProcessingVertexShader.Bind();
	mSampler.BindPS();
	mScreenQuadBuffer.Draw();
}
