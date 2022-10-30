#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ -52.6286125f,-14.4062014f,-25.0168896f });
	mCamera.SetDirection({ 0.300396204f, 0.183217078f ,-0.936052144f  });

	mPlaneMeshBuffer.Initialize(MeshBuilder::CreatePlane(1.0f, 20, 20));

	mDirectionalLight.direction = Normalize({ -0.914f, 0.261f, 0.309f });
	mDirectionalLight.direction = Normalize({ 0.985f,- 0.069f, 0.156f });
	mDirectionalLight.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mDirectionalLight.diffuse = { 0.75f,0.75f,0.75f ,1.0f };
	mDirectionalLight.specular = { 0.5f,0.5f,0.5f ,1.0f };

	mMaterial.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.diffuse = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.specular = { 0.5f,0.5f,0.5f ,1.0f };
	mMaterial.power = 80.0f;

	mModelMaterial.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mModelMaterial.diffuse = { 1.0f,1.0f,1.0f ,1.0f };
	mModelMaterial.specular = { 0.5f,0.5f,0.5f ,1.0f };
	mModelMaterial.power = 80.0f;

	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);

	// Post Processing
	auto graphicsSystem = GraphicsSystem::Get();
	mRenderTarget.Initialize(graphicsSystem->GetBackBufferWidth(), graphicsSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_F16);
	mScreenQuadBuffer.Initialize(MeshBuilder::CreateNDCQuad());

	mPostProcessingVertexShader.Initialize("../../Assets/Shaders/PostProcessing.fx", VertexPX::Format);
	mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcessing.fx", "PSNoProcessing");

	// Boat
	mBoatPosition = { 0.0f,3.5f,0.0f };
	ObjLoader::Load("../../Assets/Models/OldBoat/OldBoat.obj", 0.5f, mBoatMesh);
	mBoatMeshBuffer.Initialize(mBoatMesh);

	// Terrain
	ObjLoader::Load("../../Assets/Models/Terrain/terrain_01.obj", 0.01f, mRockyTerrain);
	mRockyTerrainBuffer.Initialize(mRockyTerrain);

	//mTranslation = { 21.3f,2.1f,43.5f };
	//mBoatPosition = { 20.4f,2.1f,40.8f };
	mTranslation = { 0.0f,-16.5f,-64.5f };
	mBoatPosition = { -48.0f,-16.8f,-34.8f };
	mRotation = { 0.0f,0.5f ,0.0f };

	mGroundEffect.Initialize();
	mGroundEffect.SetDiffuseTexture("../../Assets/Models/Terrain/textures/base.png");
	mGroundEffect.SetNormalTexture("../../Assets/Models/Terrain/textures/Normal2.png");
	mGroundEffect.SetMetallicTexture("../../Assets/Models/Terrain/textures/metallic.png");
	mGroundEffect.SetRoughnessTexture("../../Assets/Models/Terrain/textures/roughness.png");

	mBoatEffect.Initialize("../../Assets/Shaders/Standard.fx");
	mBoatEffect.SetDiffuseTexture("../../Assets/Models/OldBoat/boattex2.jpg");
	mBoatEffect.SetNormalTexture("../../Assets/Models/OldBoat/boattexnm.jpg");
	mBoatEffect.SetBrightness(2.5f);

	mWaterEffect.Initialize("../../Assets/Shaders/Water.fx");
	mWaterEffect.SetBrightness(waterBrightness);
	mWaterEffect.SetWaterDisplacement(waterDisplacement);
	mWaterEffect.SetMovementSpeed(waterMovementSpeed);
	mWaterEffect.SetReflectivePower(waterReflectionPower);

	mHdrEffect.Initialize();
	mHdrEffect.EnableHDR(false);
	mHdrEffect.EnableGammaCorrection(false);

	mSkybox.ChangeDefualtSkybox(2);
	mSkybox.CreateSkybox();
}

void GameState::Terminate()
{
	mSkybox.Terminate();

	mHdrEffect.Terminate();
	mWaterEffect.Terminate();
	mBoatEffect.Terminate();
	mGroundEffect.Terminate();

	mRockyTerrainBuffer.Terminate();
	mBoatMeshBuffer.Terminate();

	mPostProcessingPixelShader.Terminate();
	mPostProcessingVertexShader.Terminate();
	mScreenQuadBuffer.Terminate();
	mRenderTarget.Terminate();
	//
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

	mHdrEffect.BeginRender();
	mRenderTarget.BindPS(0);
	PostProcess();
	mRenderTarget.UnbindPS(0);
	mHdrEffect.EndRender();

	mHdrEffect.RenderHdrQuad();
}

void GameState::DebugUI()
{
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Refraction Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Image(
			mWaterEffect.GetRefractionTexture(),
			{ 100.0f,100.0f },
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
			{ 100.0f,100.0f },
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
		static float normalWeight = 1.5f;
		if (ImGui::Checkbox("Normal Map", &normal))
			mWaterEffect.SetNormalMapWeight(normal ? normalWeight : 0.0f);
		if(ImGui::SliderFloat("Normal Map Weight", &normalWeight, 0.0f, 50.f))
			mWaterEffect.SetNormalMapWeight(normalWeight);
		//if (ImGui::Checkbox("Specular Map", &specular))
		//	mWaterEffect.SetSpecularMapWeight(specular ? 1.0f : 0.0f);
		if(ImGui::SliderFloat("Displacement", &waterDisplacement, 0.0f, 1.0f))
			mWaterEffect.SetWaterDisplacement(waterDisplacement);
		//if(ImGui::SliderFloat("Brightness", &waterBrightness, 0.0f, 10.f))
		//	mWaterEffect.SetBrightness(waterBrightness);
		if (ImGui::SliderFloat("Movement Speed", &waterMovementSpeed, 0.0001f, 0.1f))
			mWaterEffect.SetMovementSpeed(waterMovementSpeed);
		if (ImGui::SliderFloat("Reflective Factor", &waterReflectionPower, 0.0f, 1.0f))
			mWaterEffect.SetReflectivePower(waterReflectionPower);
		ImGui::NewLine();
	}
	if (ImGui::CollapsingHeader("Transforms"))
	{
		ImGui::DragFloat3("Rotation##Transform", &mRotation.x, 0.01f);
		ImGui::DragFloat3("WaterTranslation##Transform", &mTranslation.x, 0.3f);
		ImGui::DragFloat3("BoatTranslation##Transform", &mBoatPosition.x, 0.3f);
		ImGui::DragFloat3("GroundTranslation##Transform", &mGroundTranslation.x, 0.3f);
	}
	static float exposure = 1.0f;
	//if (ImGui::SliderFloat("Exposure", &exposure, 0.0f, 10.0f))
	//{
	//	mHdrEffect.SetExposure(exposure);
	//}
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
		mClippingPlane = { 0.0f, 1.0f,0.0f, -mTranslation.y + 0.4f }; // Cull Down
		break;
	case GameState::Refraction:
		mClippingPlane = { 0.0f, -1.0f,0.0f, mTranslation.y +0.4f}; // Cull Up
		break;
	default:
		mClippingPlane = { 0.0f, 0.0f,0.0f,0.0f }; // No Cull
		break;
	}
	mBoatEffect.SetClippingPlane(mClippingPlane);
	mGroundEffect.SetClippingPlane(mClippingPlane);

	auto matTrans = Matrix4::Translation(mTranslation);
	auto matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z);
	auto matWorld = matRot * matTrans;


	//Boat
	matTrans = Matrix4::Translation({ mBoatPosition });
	matRot = Matrix4::RotationX(mBoatRotation.x) * Matrix4::RotationY(mBoatRotation.y) * Matrix4::RotationZ(mBoatRotation.z);
	matWorld = Matrix4::Scaling(0.5f)* matRot * matTrans;
	mBoatEffect.Begin();

	mBoatEffect.SetMaterial(mModelMaterial);
	mBoatEffect.SetDirectionalLight(mDirectionalLight);
	mBoatEffect.SetViewPosition(mCamera.GetPosition());
	mBoatEffect.SetWorldMatrix(matWorld);
	mBoatEffect.SetWVPMatrix(matWorld, matView, matProj);
	//mGroundEffect.SetDepthTexture(target);
	//auto wvpLight = Transpose(matWorld * lightVP);
	//mGroundEffect.UpdateShadowBuffer(wvpLight);
	mBoatEffect.UpdateSettings();

	mBoatMeshBuffer.Draw();
	mBoatEffect.End();

	// Ground
	matTrans = Matrix4::Translation({ mGroundTranslation });
	matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z);
	matWorld =  matRot * matTrans;
	mGroundEffect.Begin();
	mGroundEffect.SetMaterial(mMaterial);
	mGroundEffect.SetDirectionalLight(mDirectionalLight);
	mGroundEffect.SetViewPosition(mCamera.GetPosition());
	mGroundEffect.SetWorldMatrix(matWorld);
	mGroundEffect.SetWVPMatrix(matWorld, matView, matProj);
	//mGroundEffect.SetNormalMapWeight(1.0f);
	//mGroundEffect.SetDepthTexture(target);
	//auto wvpLight = Transpose(matWorld * lightVP);
	//mGroundEffect.UpdateShadowBuffer(wvpLight);
	mGroundEffect.UpdateSettings();

	//mPlaneMeshBuffer.Draw();
	mRockyTerrainBuffer.Draw();
	mGroundEffect.End();

	//Water
	if (rendertype == RenderType::Normal)
	{
		matTrans = Matrix4::Translation({ mTranslation });
		matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z);
		matWorld = Matrix4::Scaling({12.5f,1.0f,7.0f}) * matTrans* matRot ;

		mWaterEffect.Begin();
		mWaterEffect.SetMaterial(mMaterial);
		mWaterEffect.SetDirectionalLight(mDirectionalLight);
		mWaterEffect.SetViewPosition(mCamera.GetPosition());
		mWaterEffect.SetWorldMatrix(matWorld);
		mWaterEffect.SetWVPMatrix(matWorld, matView, matProj);
		//mWaterEffect.SetDepthTexture(target);
		//auto wvpLight = Transpose(matWorld * lightVP);
		//mWaterEffect.UpdateShadowBuffer(wvpLight);
		mWaterEffect.UpdateSettings();

		mPlaneMeshBuffer.Draw();
		mWaterEffect.End();
	}
}

void GameState::PostProcess()
{
	mPostProcessingPixelShader.Bind();
	mPostProcessingVertexShader.Bind();
	mSampler.BindPS();
	mScreenQuadBuffer.Draw();
}
