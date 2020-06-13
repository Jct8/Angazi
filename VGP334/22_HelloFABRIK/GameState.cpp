#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Physics;
using namespace Angazi::Input;
using namespace Angazi::Math;

namespace
{
	// TODO: Move this function into a new file/name: AnimationUtil and add it to the Graphics Library
	void RunFABRIK(std::vector<Vector3>& points, const Vector3& target, int maxIteration, float errorThreshold)
	{
		ASSERT(points.size() >= 2, "Not enough points to run FABRIK");

		// Record the origin
		Vector3 origin = points.front();

		// Record the currect point distances , we want to maintain these values when running IK
		std::vector<float> lengths;
		lengths.reserve(points.size() - 1);
		for (size_t i = 0; i + 1 < points.size(); ++i)
			lengths.push_back(Magnitude(points[i + 1] - points[i]));

		// Loop until we are close enough, or have used up all iteration
		for (int i = 0; i < maxIteration; i++)
		{
			if (MagnitudeSqr(points.back() -target) < Sqr(errorThreshold))
				break;

			// Run foward pass
			points.back() = target;
			for (int i = static_cast<int>(points.size()) - 2; i >= 0 ; --i)
			{
				Vector3 direction = Normalize(points[i + 1] - points[i]);
				points[i] = points[i + 1] - (direction * lengths[i]);
			}

			// Run Backward Pass
			points.front() = origin;
			for (int i = 1; i < static_cast<int>(points.size()); i++)
			{
				Vector3 direction = Normalize(points[i-1] - points[i]);
				points[i] = points[i-1] - (direction * lengths[i-1]);
			}
		}
	}
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetNearPlane(0.1f);
	mCamera.SetFarPlane(300.0f);
	mCamera.SetPosition({ 0.0f, 3.0f, -10.0f });
	mCamera.SetDirection({ 0.0f, 0.0f, 1.0f });

	const int numBones = 5;
	const float boneLength = 1.0f;

	mPoints.reserve(numBones + 1);
	mPoints.push_back(Vector3::Zero);
	for (size_t i = 1; i < mPoints.capacity(); i++)
		mPoints.push_back(mPoints.back() + Vector3{ 0.0f, boneLength, 0.0f });
	mTarget = mPoints.back();
}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 10.0f;
	const float kTurnSpeed = 10.0f * Constants::DegToRad;

	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() *kTurnSpeed*deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() *kTurnSpeed*deltaTime);
		if (inputSystem->IsKeyDown(KeyCode::W))
			mCamera.Walk(kMoveSpeed*deltaTime);
		if (inputSystem->IsKeyDown(KeyCode::S))
			mCamera.Walk(-kMoveSpeed * deltaTime);
		if (inputSystem->IsKeyDown(KeyCode::A))
			mCamera.Strafe(-kMoveSpeed * deltaTime);
		if (inputSystem->IsKeyDown(KeyCode::D))
			mCamera.Strafe(kMoveSpeed*deltaTime);
	}
	else
	{
		if (inputSystem->IsKeyDown(KeyCode::W))
			mTarget.y += kMoveSpeed * deltaTime;
		if (inputSystem->IsKeyDown(KeyCode::S))
			mTarget.y -= kMoveSpeed * deltaTime;
		if (inputSystem->IsKeyDown(KeyCode::A))
			mTarget.x -= kMoveSpeed * deltaTime;
		if (inputSystem->IsKeyDown(KeyCode::D))
			mTarget.x += kMoveSpeed * deltaTime;
		if (inputSystem->IsKeyDown(KeyCode::Q))
			mTarget.z -= kMoveSpeed * deltaTime;
		if (inputSystem->IsKeyDown(KeyCode::E))
			mTarget.z += kMoveSpeed * deltaTime;
	}

	RunFABRIK(mPoints, mTarget, 5, 0.01f);
}

void GameState::Render()
{
	for (size_t i = 0; i + 1 < mPoints.size(); ++i)
	{
		//SimpleDraw::AddBone(mPoints[i], mPoints[i + 1] - mPoints[i], Colors::Green, 0.01f, true);
		SimpleDraw::AddLine(mPoints[i], mPoints[i + 1], Colors::Green);
		SimpleDraw::AddSphere(mPoints[i], 0.1f, Colors::Cyan, false, 4, 8);
	}
	SimpleDraw::AddAABB(mTarget, { 0.2f,0.2f ,0.2f }, Colors::Red);

	SimpleDraw::AddGroundPlane(50);
	SimpleDraw::AddTransform(Matrix4::Identity);
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("FABRIK", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %.2f", Angazi::Core::TimeUtil::GetFramesPerSecond());

	ImGui::End();
}