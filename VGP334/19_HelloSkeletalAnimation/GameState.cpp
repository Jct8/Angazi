#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetNearPlane(0.1f);
	mCamera.SetFarPlane(100.0f);
	mCamera.SetPosition({ 1.20f,1.80f,-2.40f });
	mCamera.SetDirection({ -0.321f,-0.284f, 0.784f });

	mDirectionalLight.direction = Normalize({ 0.327f,-0.382f, 0.864f });
	mDirectionalLight.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mDirectionalLight.diffuse = { 0.75f,0.75f,0.75f ,1.0f };
	mDirectionalLight.specular = { 0.5f,0.5f,0.5f ,1.0f };

	mMaterial.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.diffuse = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.specular = { 0.5f,0.5f,0.5f ,1.0f };
	mMaterial.power = 80.0f;

	// Post Processing
	mPostProcessingEffect.Initialize("../../Assets/Shaders/PostProcessing.fx", "VS", "PSNoProcessing");

	// Model
	model.Initialize("../../Assets/Models/James/James.model");
	animator.Initialize(model);
	animator.SetClipLooping(0 , true);

	// Effects
	mModelStandardEffect.Initialize("../../Assets/Shaders/Standard.fx");
	mModelStandardEffect.UseShadow(true);
	mModelStandardEffect.SetNormalMapWeight(1.0f);
	mModelStandardEffect.SetSkinnedMesh(1.0f);

	mGroundStandardEffect.Initialize("../../Assets/Shaders/Standard.fx");
	mGroundStandardEffect.SetDiffuseTexture("../../Assets/Images/Floor/Stone_Tiles_004_diffuse.jpg");
	mGroundStandardEffect.SetNormalTexture("../../Assets/Images/Floor/Stone_Tiles_004_normal.jpg");
	mGroundStandardEffect.SetAOTexture("../../Assets/Images/Floor/Stone_Tiles_004_ao.jpg");
	mGroundStandardEffect.SetDisplacementTexture("../../Assets/Images/Floor/Stone_Tiles_004_height.png");
	mGroundStandardEffect.UseShadow(true);
	mGroundStandardEffect.SetBumpMapWeight(6.0f);

	mGroundMesh = MeshBuilder::CreatePlane(100.0f, 50, 50);
	mGroundMeshBuffer.Initialize(mGroundMesh);

	mShadowEffect.Initialize("../../Assets/Shaders/DepthMap.fx");
	mSkybox.CreateSkybox();
}

void GameState::Terminate()
{
	mSkybox.Terminate();
	// Effects
	mShadowEffect.Terminate();
	mGroundStandardEffect.Terminate();
	mModelStandardEffect.Terminate();

	// Model
	animator.Terminate();
	model.Terminate();

	// Post Processing
	mPostProcessingEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 10.0f;
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

	mShadowEffect.SetLightDirection(mDirectionalLight.direction, mCamera);
	if (mShowSkeleton)
		animator.PlaySkeletalAnimation(0);
	else
		animator.PlayAnimation(0);
	animator.Update(deltaTime);
}

void GameState::Render()
{
	mShadowEffect.Begin();
	DrawDepthMap();
	mShadowEffect.End();

	mPostProcessingEffect.BeginRender();
	DrawScene();
	mPostProcessingEffect.EndRender();

	mPostProcessingEffect.PostProcess();
}

void GameState::DebugUI()
{
	ImGui::Begin("Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %.2f", Angazi::Core::TimeUtil::GetFramesPerSecond());
	/*ImGui::Image(
		mShadowEffect.GetRenderTarget()->GetShaderResourceView(),
		{ 150.0f,150.0f },
		{ 0.0f,0.0f },
		{ 1.0f,1.0f },
		{ 1.0f,1.0f ,1.0f,1.0f },
		{ 1.0f,1.0f ,1.0f,1.0f }
	);*/
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
	if (ImGui::CollapsingHeader("Settings"))
	{
		static bool normal = true;
		static bool specular = true;
		static bool aoMap = true;
		//static bool useShadow = mSettings.useShadow == 1;
		if (ImGui::Checkbox("Normal Map", &normal))
		{
			float normalMapWeight = normal ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Specular Map", &specular))
		{
			float specularMapWeight = specular ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Ambient occlusion", &aoMap))
		{
			float aoMapWeight = aoMap ? 1.0f : 0.0f;
		}
	}
	if (ImGui::SliderFloat("Animation Speed", &animationSpeed, 0.0f, 50.f))
	{
		animator.SetAnimationSpeed(animationSpeed);
	}
	ImGui::Checkbox("Show Skeleton", &mShowSkeleton);

	ImGui::End();
}

void GameState::DrawScene()
{
	auto lightVP = mShadowEffect.GetVPMatrix();
	RenderTarget* target = mShadowEffect.GetRenderTarget();

	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	// Model
	auto matWorld = Matrix4::Identity;//Scaling(0.1f);
	mModelStandardEffect.Begin();
	mModelStandardEffect.SetMaterial(mMaterial);
	mModelStandardEffect.SetDirectionalLight(mDirectionalLight);
	mModelStandardEffect.SetViewProjection(mCamera.GetPosition());
	mModelStandardEffect.SetWorldMatrix(matWorld);
	mModelStandardEffect.SetWVPMatrix(matWorld, matView, matProj);
	mModelStandardEffect.SetDepthTexture(target);
	mModelStandardEffect.UpdateSettings();
	mModelStandardEffect.SetBoneTransforms(animator.GetBoneMatrices());

	if (mShowSkeleton)
		DrawSkeleton(model.skeleton, animator.GetBoneMatrices());
	else
		model.Draw(&mModelStandardEffect);
	mModelStandardEffect.End();
	SimpleDraw::Render(mCamera, matWorld);

	// Ground
	matWorld = Matrix4::Translation({ 0.0f,-2.5f,0.0f });;
	mGroundStandardEffect.Begin();
	mGroundStandardEffect.SetMaterial(mMaterial);
	mGroundStandardEffect.SetDirectionalLight(mDirectionalLight);
	mGroundStandardEffect.SetViewProjection(mCamera.GetPosition());
	mGroundStandardEffect.SetWorldMatrix(matWorld);
	mGroundStandardEffect.SetWVPMatrix(matWorld, matView, matProj);
	mGroundStandardEffect.SetDepthTexture(target);
	auto wvpLight = Transpose(matWorld * lightVP);
	mGroundStandardEffect.UpdateShadowBuffer(wvpLight);
	mGroundStandardEffect.UpdateSettings();

	mGroundMeshBuffer.Draw();
	mGroundStandardEffect.End();

	mSkybox.Draw(mCamera);
}

void GameState::DrawDepthMap()
{
	auto matWorld = Matrix4::Identity;
	mShadowEffect.SetWorldMatrix(matWorld);

	// Model
	mShadowEffect.SetWorldMatrix(matWorld);
	mShadowEffect.SetBoneTransforms(animator.GetBoneMatrices());
	mShadowEffect.SetSkinnedMesh(true);
	mShadowEffect.UpdateSettings();
	if (!mShowSkeleton)
		model.Draw(&mShadowEffect);
}