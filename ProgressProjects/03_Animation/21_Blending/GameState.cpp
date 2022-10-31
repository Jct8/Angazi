#include "GameState.h"
#include "ImGui/Inc/imgui.h"
#include "ImGui/Inc/implot.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetNearPlane(0.1f);
	mCamera.SetFarPlane(50.0f);
	mCamera.SetPosition({ -3.057f, 2.20f, -1.0f });
	mCamera.SetDirection({ 0.937f,-0.322f, 0.127f });

	mCameraOffset = mCamera.GetPosition() - mModelPosition;

	mDirectionalLight.direction = Normalize({ -0.391f,-0.353f, -0.850f });
	mDirectionalLight.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mDirectionalLight.diffuse = { 0.75f,0.75f,0.75f ,1.0f };
	mDirectionalLight.specular = { 0.5f,0.5f,0.5f ,1.0f };

	mMaterial.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.diffuse = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.specular = { 0.5f,0.5f,0.5f ,1.0f };
	mMaterial.power = 80.0f;

	mFloorMaterial.ambient = { 1.0f,1.0f,1.0f ,1.0f };
	mFloorMaterial.diffuse = { 1.0f,1.0f,1.0f ,1.0f };
	mFloorMaterial.specular = { 0.5f,0.5f,0.5f ,1.0f };
	mFloorMaterial.power = 80.0f;

	// Post Processing
	mPostProcessingEffect.Initialize(Angazi::Core::FilePath::GetAssetFilePath() + "Shaders/PostProcessing.fx", "VS", "PSNoProcessing");

	// Model
	model.Initialize(Angazi::Core::FilePath::GetAssetFilePath() + "Models/Ely/Ely.model");
	animator.Initialize(model);
	animator.SetClipLooping(1, true); // backwards left
	animator.SetClipLooping(2, true); // backwards right
	animator.SetClipLooping(3, true); // backwards
	animator.SetClipLooping(4, true); // forwards left
	animator.SetClipLooping(5, true); // forwards right
	animator.SetClipLooping(6, true); // forwards
	animator.SetClipLooping(7, true); // left
	animator.SetClipLooping(8, true); // right
	animator.SetClipLooping(0, true); // idle
	animator.PlayAnimation(0);

	animator.AddBlendAnimation({ -1,-1 }, 1);
	animator.AddBlendAnimation({ +1,-1 }, 2);
	animator.AddBlendAnimation({ 0,-1 }, 3);
	animator.AddBlendAnimation({ -1, 1 }, 4);
	animator.AddBlendAnimation({ +1, 1 }, 5);
	animator.AddBlendAnimation({ 0, 1 }, 6);
	animator.AddBlendAnimation({ -1, 0 }, 7);
	animator.AddBlendAnimation({ +1, 0 }, 8);
	//animator.AddBlendAnimation({  0,-1 }, 8);

	// Effects
	mModelStandardEffect.Initialize(Angazi::Core::FilePath::GetAssetFilePath() + "Shaders/Standard.fx");
	mModelStandardEffect.UseShadow(true);
	mModelStandardEffect.SetNormalMapWeight(1.0f);
	mModelStandardEffect.SetSkinnedMesh(1.0f);

	mGroundStandardEffect.Initialize();
	//mGroundStandardEffect.SetDiffuseTexture(Angazi::Core::FilePath::GetAssetFilePath() + "Images/Pbr/4K_Planks/Planks019_4K_Color.jpg");
	//mGroundStandardEffect.SetNormalTexture(Angazi::Core::FilePath::GetAssetFilePath() + "Images/Pbr/4K_Planks/Planks019_4K_Normal.jpg");
	//mGroundStandardEffect.SetAOTexture(Angazi::Core::FilePath::GetAssetFilePath() + "Images/Pbr/4K_Planks/Planks019_4K_AmbientOcclusion.jpg");
	//mGroundStandardEffect.SetRoughnessTexture(Angazi::Core::FilePath::GetAssetFilePath() + "Images/Pbr/4K_Planks/Planks019_4K_Roughness.jpg");
	//mGroundStandardEffect.SetMetallicTexture(Angazi::Core::FilePath::GetAssetFilePath() + "Images/Pbr/4K_Planks/Planks019_4K_Metalness.jpg");

	mGroundStandardEffect.SetDiffuseTexture(Angazi::Core::FilePath::GetAssetFilePath() + "Images/Pbr/Stylized_Stone_Floor_002_SD/Stylized_Stone_Floor_002_basecolor.jpg");
	mGroundStandardEffect.SetNormalTexture(Angazi::Core::FilePath::GetAssetFilePath() + "Images/Pbr/Stylized_Stone_Floor_002_SD/Stylized_Stone_Floor_002_normal.jpg");
	mGroundStandardEffect.SetAOTexture(Angazi::Core::FilePath::GetAssetFilePath() + "Images/Pbr/Stylized_Stone_Floor_002_SD/Stylized_Stone_Floor_002_ambientOcclusion.jpg");
	mGroundStandardEffect.SetRoughnessTexture(Angazi::Core::FilePath::GetAssetFilePath() + "Images/Pbr/Stylized_Stone_Floor_002_SD/Stylized_Stone_Floor_002_roughness.jpg");
	mGroundStandardEffect.SetMetallicTexture(Angazi::Core::FilePath::GetAssetFilePath() + "Images/Pbr/4K_Planks/Planks019_4K_Metalness.jpg");
	//mGroundStandardEffect.SetDisplacementTexture(Angazi::Core::FilePath::GetAssetFilePath() + "Images/Pbr/Stylized_Stone_Floor_002_SD/Stylized_Stone_Floor_002_height.png");

	mGroundStandardEffect.UseShadow(true);
	mGroundStandardEffect.SetBumpMapWeight(5.0f);

	mGroundMesh = MeshBuilder::CreatePlane(10.0f, 100, 100);
	mGroundMeshBuffer.Initialize(mGroundMesh);

	mShadowEffect.Initialize(Angazi::Core::FilePath::GetAssetFilePath() + "Shaders/DepthMap.fx");
	mSkybox.CreateSkybox();

	mHdrEffect.Initialize();
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);
}

void GameState::Terminate()
{
	mSampler.Terminate();
	mHdrEffect.Terminate();
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

	if ((inputSystem->IsKeyDown(KeyCode::W) || inputSystem->IsKeyDown(KeyCode::S)))
	{
		if (inputSystem->IsKeyDown(KeyCode::W))
			mInputAxis.y += deltaTime;
		if (inputSystem->IsKeyDown(KeyCode::S))
			mInputAxis.y -= deltaTime;
	}
	else
	{
		if (fabs(mInputAxis.y) <= 0.02f)
			mInputAxis.y = 0.0f;
		else if (mInputAxis.y > 0.0f)
			mInputAxis.y -= deltaTime;
		else if (mInputAxis.y < 0.0f)
			mInputAxis.y += deltaTime;
	}
	if ((inputSystem->IsKeyDown(KeyCode::A) || inputSystem->IsKeyDown(KeyCode::D)))
	{
		if (inputSystem->IsKeyDown(KeyCode::A))
			mInputAxis.x -= deltaTime;
		if (inputSystem->IsKeyDown(KeyCode::D))
			mInputAxis.x += deltaTime;
	}
	else
	{
		if (fabs(mInputAxis.x) <= 0.02f)
			mInputAxis.x = 0.0f;
		else if (mInputAxis.x > 0.0f)
			mInputAxis.x -= deltaTime;
		else if (mInputAxis.x < 0.0f)
			mInputAxis.x += deltaTime;
	}
	if (mInputAxis.x == 0.0f && mInputAxis.y == 0.0f)
		animator.BlendTo(0, 0.5f);
	else
		animator.SetBlendVelocity(mInputAxis);

	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() * kTurnSpeed * deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() * kTurnSpeed * deltaTime);
	}

	mInputAxis.x = Clamp(mInputAxis.x, -1.0f, 1.0f);
	mInputAxis.y = Clamp(mInputAxis.y, -1.0f, 1.0f);

	Vector2 movement = mInputAxis * deltaTime * mMovementSpeed;
	mModelPosition.x -= movement.x;
	mModelPosition.z -= movement.y;

	mCamera.SetPosition(mModelPosition + mCameraOffset);
	mShadowEffect.SetLightDirection(mDirectionalLight.direction, mCamera);

	animator.Update(deltaTime);
}

void GameState::Render()
{
	mShadowEffect.Begin();
	DrawDepthMap();
	mShadowEffect.End();

	//mPostProcessingEffect.BeginRender();
	DrawScene();
	//mPostProcessingEffect.EndRender();

	//mHdrEffect.BeginRender();
	//mPostProcessingEffect.PostProcess();
	//mHdrEffect.EndRender();
	//
	//mHdrEffect.RenderHdrQuad();
}

void GameState::DebugUI()
{
	ImGui::Begin("Settings", nullptr);
	//ImGui::Text("FPS: %.2f", Angazi::Core::TimeUtil::GetFramesPerSecond());
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
	//if (ImGui::CollapsingHeader("Material"))
	//{
	//	ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.x);
	//	ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.x);
	//	ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.x);
	//	ImGui::DragFloat("Power##Material", &mMaterial.power, 1.0f, 1.0f, 100.0f);
	//}
	//if (ImGui::CollapsingHeader("Settings"))
	//{
	//	static bool normal = true;
	//	static bool specular = true;
	//	static bool aoMap = true;
	//	//static bool useShadow = mSettings.useShadow == 1;
	//	if (ImGui::Checkbox("Normal Map", &normal))
	//	{
	//		float normalMapWeight = normal ? 1.0f : 0.0f;
	//	}
	//	if (ImGui::Checkbox("Specular Map", &specular))
	//	{
	//		float specularMapWeight = specular ? 1.0f : 0.0f;
	//	}
	//	if (ImGui::Checkbox("Ambient occlusion", &aoMap))
	//	{
	//		float aoMapWeight = aoMap ? 1.0f : 0.0f;
	//	}
	//}

	if (ImGui::CollapsingHeader("Animation",ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::SliderFloat("Animation Speed", &animationSpeed, 0.0f, 6.0f))
			animator.SetAnimationSpeed(animationSpeed);
		ImGui::SliderFloat("Movement Speed", &mMovementSpeed, 0.0f, 8.0f);
		//ImGui::DragFloat("X axis", &mInputAxis.x, 0.1f, -1.0f, 1.0f);
		//ImGui::DragFloat("Y axis", &mInputAxis.y, 0.1f, -1.0f, 1.0f);
		if (ImGui::Checkbox("Show Skeleton", &mShowSkeleton))
			animator.SetShowSkeleton(mShowSkeleton);
		ImGui::Separator();
		constexpr float pointsX[8] = {-1,0,1,-1,1,-1, 0, 1 };
		constexpr float pointsY[8] = { 1,1,1, 0,0,-1,-1,-1 };
		constexpr float center = 0.0f;
		ImPlot::SetNextPlotLimits(-1.1, 1.1, -1.1, 1.1,ImGuiCond_Always);
		if (ImPlot::BeginPlot("2D Direction Blend Tree", NULL, NULL, {-1,0},
			ImPlotFlags_NoLegend | ImPlotFlags_AntiAliased | ImPlotFlags_NoMousePos | ImPlotFlags_NoBoxSelect))
		{
			ImPlot::SetLegendLocation(ImPlotLocation_East,1,true);
			ImPlot::PlotText("Strafe\nLeft", -0.75, 0);
			ImPlot::PlotText("Strafe\nRight", 0.75, 0);
			ImPlot::PlotText("Forwards", 0, 0.9);
			ImPlot::PlotText("Backwards", 0, -0.9);
			ImPlot::PlotScatter("Animation Clips", pointsX, pointsY, 8);
			ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
			ImPlot::SetNextMarkerStyle(ImPlotMarker_Diamond, 6, ImVec4(1, 0, 0, 0.75f), IMPLOT_AUTO, ImVec4(1, 0, 0, 0.75f));
			ImPlot::PlotScatter("Blend Weight", &mInputAxis.x, &mInputAxis.y, 1);
			ImPlot::PopStyleVar();
			ImPlot::EndPlot();
		}
		ImGui::SliderFloat("Forward", &mInputAxis.y, -1.0f, 1.0f,"%.2f");
		ImGui::SliderFloat("Strafe", &mInputAxis.x,-1.0f, 1.0f, "%.2f");

	}
	ImGui::End();
}

void GameState::DrawScene()
{
	auto lightVP = mShadowEffect.GetVPMatrix();
	RenderTarget* target = mShadowEffect.GetRenderTarget();

	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	// Model
	auto matWorld = Matrix4::Translation(mModelPosition);
	mModelStandardEffect.Begin();
	mModelStandardEffect.SetMaterial(mMaterial);
	mModelStandardEffect.SetDirectionalLight(mDirectionalLight);
	mModelStandardEffect.SetViewPosition(mCamera.GetPosition());
	mModelStandardEffect.SetWorldMatrix(matWorld);
	mModelStandardEffect.SetWVPMatrix(matWorld, matView, matProj);
	mModelStandardEffect.SetDepthTexture(target);
	mModelStandardEffect.UpdateSettings();
	mModelStandardEffect.SetBoneTransforms(animator.GetBoneMatrices());

	if (mShowSkeleton)
		DrawSkeleton(model.skeleton, animator.GetBoneMatrices(), 3.0f);
	else
		model.Draw(&mModelStandardEffect);
	mModelStandardEffect.End();
	SimpleDraw::Render(mCamera, matWorld);

	// Ground
	matWorld = Matrix4::Translation({ 0.0f,-0.0f,0.0f });;
	mGroundStandardEffect.Begin();
	mSampler.BindPS();
	mSampler.BindVS();
	mGroundStandardEffect.SetMaterial(mFloorMaterial);
	mGroundStandardEffect.SetDirectionalLight(mDirectionalLight);
	mGroundStandardEffect.SetViewPosition(mCamera.GetPosition());
	mGroundStandardEffect.SetWorldMatrix(matWorld);
	mGroundStandardEffect.SetWVPMatrix(matWorld, matView, matProj);
	mGroundStandardEffect.SetDepthTexture(target);
	auto wvpLight = Transpose(matWorld * lightVP);
	mGroundStandardEffect.UpdateShadowBuffer(wvpLight);
	mGroundStandardEffect.UpdateSettings();

	mGroundMeshBuffer.Draw();
	mGroundStandardEffect.End();

	//SimpleDraw::AddGroundPlane(100, false);
	SimpleDraw::Render(mCamera);

	mSkybox.Draw(mCamera);
}

void GameState::DrawDepthMap()
{
	auto matWorld = Matrix4::Translation(mModelPosition);
	mShadowEffect.SetWorldMatrix(matWorld);

	// Model
	mShadowEffect.SetWorldMatrix(matWorld);
	mShadowEffect.SetBoneTransforms(animator.GetBoneMatrices());
	mShadowEffect.SetSkinnedMesh(true);
	mShadowEffect.UpdateSettings();
	if (!mShowSkeleton)
		model.Draw(&mShadowEffect);
}