#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Physics;
using namespace Angazi::Input;
using namespace Angazi::Math;

namespace
{

}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetNearPlane(0.1f);
	mCamera.SetFarPlane(300.0f);
	mCamera.SetPosition({ 0.0f, 23.0f, -33.0f });
	mCamera.SetDirection({ 0.0f,-0.36f, 0.92f });

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

	// Physics
	Physics::PhysicsWorld::Settings settings;
	settings.drag = 0.1f;
	mPhysicsWorld.Initialize(settings);
	mPhysicsWorld.AddStaticPlane({ Vector3::YAxis,0.0f });
	//mPhysicsWorld.AddStaticOBB({ { 2.0f,2.0f,0.0f }, {4.0f,0.2f,5.0f} , Quaternion::RotationAxis(Vector3::ZAxis, 10.0f * Constants::DegToRad) });
	//mPhysicsWorld.AddStaticOBB({ {-3.0f,4.5f,0.0f }, {4.0f,0.2f,5.0f} , Quaternion::RotationAxis(Vector3::ZAxis,-15.0f * Constants::DegToRad) });

	// Model
	mModel.Initialize("../../Assets/Models/James/James.model");
	animator.Initialize(mModel);
	animator.SetClipLooping(0, true);

	// Effects
	mModelStandardEffect.Initialize("../../Assets/Shaders/Standard.fx");
	mModelStandardEffect.UseShadow(true);
	mModelStandardEffect.SetNormalMapWeight(1.0f);
	mModelStandardEffect.SetSkinnedMesh(1.0f);
}

void GameState::Terminate()
{
	mPhysicsWorld.Clear();
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

	if (mShowSkeleton)
		animator.PlaySkeletalAnimation(0);
	else
		animator.PlayAnimation(0);
	animator.Update(deltaTime);

	mPhysicsWorld.Update(deltaTime);
}

void GameState::Render()
{
	DrawScene();
}

void GameState::DebugUI()
{
	ImGui::Begin("Rag doll", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %.2f", Angazi::Core::TimeUtil::GetFramesPerSecond());
	ImGui::Checkbox("Show Skeleton", &mShowSkeleton);
	ImGui::End();
}

void GameState::DrawScene()
{
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
	mModelStandardEffect.UpdateSettings();
	mModelStandardEffect.SetBoneTransforms(animator.GetBoneMatrices());

	if (mShowSkeleton)
		DrawSkeleton(mModel.skeleton, animator.GetBoneMatrices());
	else
		mModel.Draw(&mModelStandardEffect);
	mModelStandardEffect.End();

	mPhysicsWorld.DebugDraw();

	SimpleDraw::AddGroundPlane(50);
	SimpleDraw::Render(mCamera);
}