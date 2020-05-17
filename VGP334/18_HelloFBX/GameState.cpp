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

	mCamera.SetNearPlane(0.1f);
	mCamera.SetFarPlane(300.0f);
	/*mCamera.SetPosition({ 0.0f, 10.0f, -30.0f });
	mCamera.SetDirection({ 0.0f,0.0f, 1.0f });*/

	mDirectionalLight.direction = Normalize({ -0.914f, 0.261f, 0.309f });
	mDirectionalLight.direction = Normalize({ 0.985f,-0.069f, 0.156f });
	mDirectionalLight.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mDirectionalLight.diffuse = { 0.75f,0.75f,0.75f ,1.0f };
	mDirectionalLight.specular = { 0.5f,0.5f,0.5f ,1.0f };

	mMaterial.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.diffuse = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.specular = { 0.5f,0.5f,0.5f ,1.0f };
	mMaterial.power = 80.0f;

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

	mAnimation = Graphics::AnimationBuilder()
		.SetTime(0.0f)
			.AddPositionKey(Math::Vector3(-20.0f, 0.0f, 0.0f))
			.AddRotationKey(Math::Quaternion::Identity)
			.AddScaleKey(Math::Vector3::One)
		.SetTime(5.0f)
			.AddPositionKey(Math::Vector3(0.0f, 20.0f, 20.0f))
			.AddRotationKey(Math::Quaternion::RotationAxis(Math::Vector3(-1.0f, 1.0f, -1.0f), Math::Constants::DegToRad * 40.0f))
		.SetTime(10.0f)
			.AddPositionKey(Math::Vector3(20.0f, 0.0f, 0.0f))
			.AddRotationKey(Math::Quaternion::RotationAxis(Math::Vector3(0.0f, 1.0f, 0.0f), Math::Constants::DegToRad * 180.0f))
		.SetTime(15.0f)
			.AddPositionKey(Math::Vector3(0.0f, -20.0f, -20.0f))
			.AddRotationKey(Math::Quaternion::RotationAxis(Math::Vector3(-1.0f, 1.0f, -1.0f), Math::Constants::DegToRad * 270.0f))
		.SetTime(20.0f)
			.AddPositionKey(Math::Vector3(-20.0f, 0.0f, 0.0f))
			.AddRotationKey(Math::Quaternion::Identity)
		.SetLooping(true)
		.Build();

	model.Initialize("../../Assets/Models/Chad/Chad.model");
	mBoneMatrices.resize(model.skeleton.bones.size());
	ComputeBoneMatrices(model.skeleton.root, mBoneMatrices);

	// Effects
	mJetStandardEffect.Initialize("../../Assets/Shaders/Standard.fx");
	mJetStandardEffect.SetDiffuseTexture("../../Assets/Models/Jet/F 15E.jpg");
	mJetStandardEffect.SetSpecularTexture("../../Assets/Models/Jet/F 15 Specular.jpg");
	mJetStandardEffect.SetNormalTexture("../../Assets/Models/Jet/F-15C normal.jpg");
	mJetStandardEffect.SetAOTexture("../../Assets/Models/Jet/F-15CAO.jpg");
	mJetStandardEffect.UseShadow(true);

	mModelStandardEffect.Initialize("../../Assets/Shaders/Standard.fx");
	mModelStandardEffect.UseShadow(true);

	mGroundStandardEffect.Initialize("../../Assets/Shaders/Standard.fx");
	mGroundStandardEffect.SetDiffuseTexture("../../Assets/Images/Sand.jpg");
	mGroundStandardEffect.UseShadow(true);

	mGroundMesh = MeshBuilder::CreatePlane(100.0f, 200, 200);
	mGroundMeshBuffer.Initialize(mGroundMesh);

	//mShadowEffect.Initialize("../../Assets/Shaders/DepthMap.fx");
}

void GameState::Terminate()
{
	mShadowEffect.Terminate();
	mGroundStandardEffect.Terminate();
	mModelStandardEffect.Terminate();
	mJetStandardEffect.Terminate();

	// Model
	model.Terminate();
	mJetMeshBuffer.Terminate();

	mPostProcessingPixelShader.Terminate();
	mPostProcessingVertexShader.Terminate();
	mScreenQuadBuffer.Terminate();
	mRenderTarget.Terminate();
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
	//mShadowEffect.SetLightDirection(mDirectionalLight.direction, mCamera);
}

void GameState::Render()
{
	//mShadowEffect.Begin();
	//DrawDepthMap();
	//mShadowEffect.End();
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
	//ImGui::Image(
	//	mShadowEffect.GetRenderTarget()->GetShaderResourceView(),
	//	{ 150.0f,150.0f },
	//	{ 0.0f,0.0f },
	//	{ 1.0f,1.0f },
	//	{ 1.0f,1.0f ,1.0f,1.0f },
	//	{ 1.0f,1.0f ,1.0f,1.0f }
	//);
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
		//ImGui::SliderFloat("Brightness", &mSettings.brightness, 0.0f, 10.f);
	}
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("JetTranslation##Transform", &mJetPosition.x, 0.3f);
	}
	ImGui::Checkbox("Show Skeleton", &mShowSkeleton);

	ImGui::End();
}

void GameState::DrawScene()
{
	//auto lightVP = mShadowEffect.GetVPMatrix();
	//RenderTarget* target = mShadowEffect.GetRenderTarget();
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	// Jet
	auto matWorld = mAnimation.GetTransform(dt);
	mJetStandardEffect.Begin();
	mJetStandardEffect.SetMaterial(mMaterial);
	mJetStandardEffect.SetDirectionalLight(mDirectionalLight);
	mJetStandardEffect.SetViewProjection(mCamera.GetPosition());
	mJetStandardEffect.SetWorldMatrix(matWorld);
	mJetStandardEffect.SetWVPMatrix(matWorld,matView,matProj);
	mJetStandardEffect.UpdateSettings();

	mJetMeshBuffer.Draw();
	mJetStandardEffect.End();

	// Model
	auto matRot = Matrix4::RotationY(Constants::Pi);
	matWorld = Matrix4::Scaling(0.1f) * matRot * matWorld;
	mModelStandardEffect.Begin();
	mModelStandardEffect.SetMaterial(mMaterial);
	mModelStandardEffect.SetDirectionalLight(mDirectionalLight);
	mModelStandardEffect.SetViewProjection(mCamera.GetPosition());
	mModelStandardEffect.SetWorldMatrix(matWorld);
	mModelStandardEffect.SetWVPMatrix(matWorld, matView, matProj);
	//mModelStandardEffect.SetDepthTexture(target);
	mModelStandardEffect.UpdateSettings();

	if (mShowSkeleton)
		DrawSkeleton(model.skeleton, mBoneMatrices);
	else
		model.Draw();

	mModelStandardEffect.End();

	SimpleDraw::Render(mCamera, matWorld);

	// Ground
	matWorld = Matrix4::Translation({ 0.0f,-20.0f,0.0f });;
	mGroundStandardEffect.Begin();
	mGroundStandardEffect.SetMaterial(mMaterial);
	mGroundStandardEffect.SetDirectionalLight(mDirectionalLight);
	mGroundStandardEffect.SetViewProjection(mCamera.GetPosition());
	mGroundStandardEffect.SetWorldMatrix(matWorld);
	mGroundStandardEffect.SetWVPMatrix(matWorld, matView, matProj);
	//mGroundStandardEffect.SetDepthTexture(target);
	//auto wvpLight = Transpose(matWorld * lightVP);
	//mGroundStandardEffect.UpdateShadowBuffer(wvpLight);

	mGroundStandardEffect.UpdateSettings();

	//mGroundMeshBuffer.Draw();
	mGroundStandardEffect.End();

}

void GameState::PostProcess()
{
	mPostProcessingPixelShader.Bind();
	mPostProcessingVertexShader.Bind();
	mSampler.BindPS();
	mScreenQuadBuffer.Draw();
}

void GameState::DrawDepthMap()
{
	// Jet
	auto matWorld = mAnimation.GetTransform(dt);
	mShadowEffect.SetWorldMatrix(matWorld);
	mJetMeshBuffer.Draw();

	// Model
	auto matRot = Matrix4::RotationY(Constants::Pi);
	matWorld = Matrix4::Scaling(0.1f) * matRot * matWorld;
	mShadowEffect.SetWorldMatrix(matWorld);

	if (mShowSkeleton)
		DrawSkeleton(model.skeleton, mBoneMatrices);
	else
		model.Draw();
}