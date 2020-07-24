#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Physics;
using namespace Angazi::Input;
using namespace Angazi::Math;

namespace
{
	std::vector<Angazi::Physics::Particle*> particlesStack;
	std::vector<Angazi::Physics::Particle*> mParticles;
	std::vector<Math::Matrix4> mBoneMatrices;
	std::vector<Math::Matrix4> mAnimationMatrices;
	int num = 0;

	void ConstructFigure(Bone* bone, PhysicsWorld& physicsWorld)
	{
		Math::Vector3 bonePositiion = GetTranslation(mAnimationMatrices[bone->index]);

		auto particle = new Physics::Particle();
		//bonePositiion.y += 0.2f;
		particle->SetPosition(bonePositiion);
		particle->SetVelocity({ -0.04f,0.1f,0.04f });
		particle->radius = 0.01f;

		//if (num >=1)
		{
			physicsWorld.AddParticles(particle);
			if (!particlesStack.empty())
				physicsWorld.AddConstraint(new Spring(particle, particlesStack.back()));
		}
		mParticles[bone->index] = particle;
		particlesStack.push_back(particle);

		for (size_t i = 0; i < bone->children.size(); i++)
		{
			num++;
			ConstructFigure(bone->children[i], physicsWorld);
		}
		particlesStack.pop_back();
		num--;
	}



	void GetBoneMatrices(Bone* bone, std::vector<Math::Matrix4>& boneMatrices, const PhysicsWorld& physicsWorld)
	{
		Math::Vector3 bonePosition = mParticles[bone->index]->position;
		Math::Vector3 boneParentPosition = bone->parentIndex != -1 ? mParticles[bone->parentIndex]->position : bonePosition;

		//Math::Vector3 originalDirection = bone->parentIndex != -1 
		//	? GetLook(mBoneMatrices[bone->parentIndex])
		Math::Vector3 originalDirection = GetLook(mBoneMatrices[bone->index]);
		originalDirection = bone->parentIndex != -1 ? GetLook(mBoneMatrices[bone->parentIndex]) : Vector3::YAxis;
		//Math::Vector3 originalDirectionUp = bone->parentIndex != -1
		//	? GetUp(mBoneMatrices[bone->parentIndex])
		Math::Vector3 originalDirectionUp = GetUp(mBoneMatrices[bone->index]);
		//Math::Vector3 originalDirection = bone->parentIndex != -1 ?
		//	GetTranslation(mBoneMatrices[bone->parentIndex]) - GetTranslation(mBoneMatrices[bone->index])
		//	: GetTranslation(mBoneMatrices[bone->index]);
		Math::Vector3 newDirection = bonePosition -boneParentPosition ;
		//Math::Vector3 newDirection = bone->parentIndex != -1 ?
		//	GetTranslation(mAnimationMatrices[bone->parentIndex]) - GetTranslation(mAnimationMatrices[bone->index])
		//	: GetTranslation(mAnimationMatrices[bone->index]);
		//Math::Vector3 newDirection  = bone->parentIndex != -1
		//	? GetLook(mAnimationMatrices[bone->parentIndex])
		//Math::Vector3 newDirection = GetLook(mAnimationMatrices[bone->index]);
		//Math::Vector3 newDirectionUp = bone->parentIndex != -1
		//	? GetUp(mAnimationMatrices[bone->parentIndex])
		//Math::Vector3 newDirectionUp = GetUp(mAnimationMatrices[bone->index]);
		Math::Vector3 newDirectionUp = bone->parentIndex != -1 ? GetRight(boneMatrices[bone->parentIndex]) : Vector3::YAxis;
		newDirectionUp = Normalize(Cross( boneParentPosition, bonePosition));

		//SimpleDraw::AddBone(boneParentPosition, newDirection, Colors::AliceBlue, 0.01f, true);
		//SimpleDraw::AddBone(GetTranslation(mBoneMatrices[bone->index]), newDirection, Colors::Red, 0.01f, true);
		//auto m1 = GetRotation(mAnimationMatrices[bone->index]);
		//auto m2 = GetRotation(mBoneMatrices[bone->index]);
		Quaternion rotation = Quaternion::RotationLookAt(newDirectionUp, newDirection);
		Matrix4 rotationMatrix = Matrix4::RotationQuaternion(rotation);
		if (newDirection.x == 0.0f && newDirection.y == 0.0f && newDirection.z == 0.0f && bone->parentIndex != -1)
		{
			rotationMatrix = boneMatrices[bone->parentIndex];
			rotationMatrix._41 = 0.0f;
			rotationMatrix._42 = 0.0f;
			rotationMatrix._43 = 0.0f;
		}
		//- Quaternion::RotationLookAt(originalDirection, originalDirectionUp)) ) ;
		//Quaternion rotation = Normalize(Quaternion::RotationMatrix(m1) - Quaternion::RotationMatrix(m2));
		//Quaternion rotation = Quaternion::RotationFromTo(originalDirection, (newDirection));
		//auto newMat =  Matrix4::Translation(trans) * Matrix4::RotationQuaternion(rotation);
		auto boneMatrix = mBoneMatrices[bone->index];
		boneMatrix._41 = 0.0f;
		boneMatrix._42 = 0.0f;
		boneMatrix._43 = 0.0f;
		boneMatrices[bone->index] = /*boneMatrix **/ rotationMatrix * Matrix4::Translation(bonePosition);

		for (size_t i = 0; i < bone->children.size(); i++)
			GetBoneMatrices(bone->children[i], boneMatrices, physicsWorld);

		//boneMatrices[bone->index] = Transpose(bone->offsetTransform *boneMatrices[bone->index]) ;
	}
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
	//settings.gravity.y = 0.0f;
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

	// Pose Matrix
	mBoneMatrices = animator.GetBoneMatrices();
	mParticles.resize(mModel.skeleton.bones.size());

	// Animation Matrix
	animator.PlaySkeletalAnimation(0);
	animator.Update(0.0f);
	mAnimationMatrices = animator.GetBoneMatrices();

	ConstructFigure(mModel.skeleton.root, mPhysicsWorld);
}

void GameState::Terminate()
{
	mPhysicsWorld.Clear();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
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

	//if (mShowSkeleton)
	//	animator.PlaySkeletalAnimation(0);
	//else
	//	animator.PlayAnimation(0);
	animator.ComputeBindPose();
	//animator.Update(deltaTime);

	if (mApplyPhysics)
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
	ImGui::Checkbox("Update Physics", &mApplyPhysics);
	if (ImGui::Button("Reset Figure"))
	{
		mPhysicsWorld.Clear(true);
		animator.ComputeBindPose();
		auto boneMatrices = animator.GetBoneMatrices();
		ConstructFigure(mModel.skeleton.root, mPhysicsWorld);
	}
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
	mModelStandardEffect.SetViewPosition(mCamera.GetPosition());
	mModelStandardEffect.SetWorldMatrix(matWorld);
	mModelStandardEffect.SetWVPMatrix(matWorld, matView, matProj);
	mModelStandardEffect.UpdateSettings();

	auto boneMatrices = animator.GetBoneMatrices();
	GetBoneMatrices(mModel.skeleton.root, boneMatrices, mPhysicsWorld);
	mModelStandardEffect.SetBoneTransforms(boneMatrices);

	if (mShowSkeleton)
		DrawSkeleton(mModel.skeleton, boneMatrices);
	else
		DrawSkeleton(mModel.skeleton, mAnimationMatrices);
		//mModel.Draw(&mModelStandardEffect);
	mModelStandardEffect.End();

	mPhysicsWorld.DebugDraw();

	//SimpleDraw::AddGroundPlane(50);
	SimpleDraw::Render(mCamera);
}