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

	mPlaneMeshBuffer.Initialize(MeshBuilder::CreatePlane(200.0f, 20, 20));

	mDirectionalLight.direction = Normalize({ -0.914f, 0.261f, 0.309f });
	mDirectionalLight.direction = Normalize({ 0.985f,- 0.069f, 0.156f });
	mDirectionalLight.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mDirectionalLight.diffuse = { 0.75f,0.75f,0.75f ,1.0f };
	mDirectionalLight.specular = { 0.5f,0.5f,0.5f ,1.0f };

	mMaterial.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.diffuse = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.specular = { 0.5f,0.5f,0.5f ,1.0f };
	mMaterial.power = 80.0f;

	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
	mBlendState.Initialize(BlendState::Mode::Additive);

	// Post Processing
	auto graphicsSystem = GraphicsSystem::Get();
	mRenderTarget.Initialize(graphicsSystem->GetBackBufferWidth(), graphicsSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);
	mScreenQuadBuffer.Initialize(MeshBuilder::CreateNDCQuad());

	mPostProcessingVertexShader.Initialize("../../Assets/Shaders/PostProcessing.fx", VertexPX::Format);
	mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcessing.fx", "PSNoProcessing");

	// Tank
	mTankPosition = { 0.0f,3.5f,0.0f };
	ObjLoader::Load("../../Assets/Models/Tank/tank.obj", 0.001f, mTankMesh);
	mTankMeshBuffer.Initialize(mTankMesh);

	// Terrain
	mTerrain.Initialize(200, 200, 1.0f);
	mTerrain.SetHeightScale(30.0f);
	mTerrain.LoadHeightmap("../../Assets/Heightmaps/heightmap_200x200.raw");

	mTranslation = { 21.3f,2.1f,43.5f };
	mTankPosition = { 18.0f,3.8f,40.2f };

	mGroundEffect.Initialize("../../Assets/Shaders/Standard.fx");
	mGroundEffect.SetDiffuseTexture("../../Assets/Images/grass.jpg");

	mTankEffect.Initialize("../../Assets/Shaders/Standard.fx");
	mTankEffect.SetDiffuseTexture("../../Assets/Models/Tank/tank_diffuse.jpg");
	mTankEffect.SetSpecularTexture("../../Assets/Models/Tank/tank_specular.jpg");
	mTankEffect.SetNormalTexture("../../Assets/Models/Tank/tank_normal.jpg");
	mTankEffect.SetAOTexture("../../Assets/Models/Tank/tank_ao.jpg");
	mTankEffect.SetBrightness(10.0f);

	mWaterEffect.Initialize("../../Assets/Shaders/Water.fx");
	mWaterEffect.SetBrightness(waterBrightness);
	mWaterEffect.SetWaterDisplacement(waterDisplacement);
	mWaterEffect.SetMovementSpeed(waterMovementSpeed);

	mSkybox.CreateSkybox();
}

void GameState::Terminate()
{
	mSkybox.Terminate();

	mTankEffect.Terminate();
	mGroundEffect.Terminate();

	mTerrain.Terminate();
	mTankMeshBuffer.Terminate();

	mPostProcessingPixelShader.Terminate();
	mPostProcessingVertexShader.Terminate();
	mScreenQuadBuffer.Terminate();
	mRenderTarget.Terminate();
	//
	mBlendState.Terminate();
	mSampler.Terminate();

	mPlaneMeshBuffer.Terminate();
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

	mWaterEffect.Update(deltaTime);
}

void GameState::Render()
{
	/////Refraction///////
	mWaterEffect.BeginRefraction();
	DrawScene(RenderType::Refraction);
	mWaterEffect.EndRefraction();


	/////Reflection///////
	mWaterEffect.BeginReflection(mCamera,mTranslation.y);
	DrawScene(RenderType::Reflection);
	mWaterEffect.EndReflection(mCamera);


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
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Refraction Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Image(
			mWaterEffect.GetRefractionTexture(),
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
			mWaterEffect.GetReflectionTexture(),
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
	if (ImGui::CollapsingHeader("Water Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		static bool normal = true;
		static bool specular = true;
		static bool aoMap = true;
		if(ImGui::SliderFloat("Displacement", &waterDisplacement, 0.0f, 1.0f))
			mWaterEffect.SetWaterDisplacement(waterDisplacement);
		if (ImGui::Checkbox("Normal Map", &normal))
			mWaterEffect.SetNormalMapWeight(normal ? 1.0f : 0.0f);
		if (ImGui::Checkbox("Specular Map", &specular))
			mWaterEffect.SetSpecularMapWeight(specular ? 1.0f : 0.0f);
		if(ImGui::SliderFloat("Brightness", &waterBrightness, 0.0f, 10.f))
			mWaterEffect.SetBrightness(waterBrightness);
		if (ImGui::SliderFloat("Movement Speed", &waterMovementSpeed, 0.0001f, 0.1f))
			mWaterEffect.SetMovementSpeed(waterMovementSpeed);
	}
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Rotation##Transform", &mRotation.x, 0.01f);
		ImGui::DragFloat3("WaterTranslation##Transform", &mTranslation.x, 0.3f);
		ImGui::DragFloat3("TankTranslation##Transform", &mTankPosition.x, 0.3f);
		ImGui::DragFloat3("GroundTranslation##Transform", &mGroundTranslation.x, 0.3f);
	}
	ImGui::End();
}

void GameState::DrawScene(RenderType rendertype)
{
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	mSkybox.Draw(mCamera);
	mSampler.BindVS();
	mSampler.BindPS();

	switch (rendertype)
	{
	case GameState::Reflection:
		mClippingPlane = { 0.0f, 1.0f,0.0f, -mTranslation.y }; // Cull Down
		break;
	case GameState::Refraction:
		mClippingPlane = { 0.0f, -1.0f,0.0f, mTranslation.y }; // Cull Up
		break;
	default:
		mClippingPlane = { 0.0f, 0.0f,0.0f,0.0f }; // No Cull
		break;
	}
	mTerrain.SetClippingPlane(mClippingPlane);
	mTankEffect.SetClippingPlane(mClippingPlane);
	mGroundEffect.SetClippingPlane(mClippingPlane);

	auto matTrans = Matrix4::Translation(mTranslation);
	auto matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z);
	auto matWorld = matRot * matTrans;

	//Water
	if (rendertype == RenderType::Normal)
	{
		matTrans = Matrix4::Translation({ mTranslation });
		matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z);
		matWorld = matRot * matTrans;

		mWaterEffect.Begin();
		mWaterEffect.SetMaterial(mMaterial);
		mWaterEffect.SetDirectionalLight(mDirectionalLight);
		mWaterEffect.SetViewProjection(mCamera.GetPosition());
		mWaterEffect.SetWorldMatrix(matWorld);
		mWaterEffect.SetWVPMatrix(matWorld, matView, matProj);
		//mWaterEffect.SetDepthTexture(target);
		//auto wvpLight = Transpose(matWorld * lightVP);
		//mWaterEffect.UpdateShadowBuffer(wvpLight);
		mWaterEffect.UpdateSettings();

		mBlendState.Bind();
		mPlaneMeshBuffer.Draw();
		mWaterEffect.End();
		mBlendState.ClearState();
	}

	//Tank
	matTrans = Matrix4::Translation({ mTankPosition });
	matRot = Matrix4::RotationX(mTankRotation.x) * Matrix4::RotationY(mTankRotation.y) * Matrix4::RotationZ(mTankRotation.z);
	matWorld = Matrix4::Scaling(0.5f)* matRot * matTrans;
	mTankEffect.Begin();
	mTankEffect.SetMaterial(mMaterial);
	mTankEffect.SetDirectionalLight(mDirectionalLight);
	mTankEffect.SetViewProjection(mCamera.GetPosition());
	mTankEffect.SetWorldMatrix(matWorld);
	mTankEffect.SetWVPMatrix(matWorld, matView, matProj);
	//mGroundEffect.SetDepthTexture(target);
	//auto wvpLight = Transpose(matWorld * lightVP);
	//mGroundEffect.UpdateShadowBuffer(wvpLight);
	mTankEffect.UpdateSettings();

	mTankMeshBuffer.Draw();
	mTankEffect.End();

	// Ground
	matTrans = Matrix4::Translation({ mGroundTranslation });
	matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z);
	matWorld =  matRot * matTrans;
	mGroundEffect.Begin();
	mGroundEffect.SetMaterial(mMaterial);
	mGroundEffect.SetDirectionalLight(mDirectionalLight);
	mGroundEffect.SetViewProjection(mCamera.GetPosition());
	mGroundEffect.SetWorldMatrix(matWorld);
	mGroundEffect.SetWVPMatrix(matWorld, matView, matProj);
	//mGroundEffect.SetDepthTexture(target);
	//auto wvpLight = Transpose(matWorld * lightVP);
	//mGroundEffect.UpdateShadowBuffer(wvpLight);
	mGroundEffect.UpdateSettings();

	mPlaneMeshBuffer.Draw();
	mGroundEffect.End();

	// Terrain
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
