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
	mCamera.SetFarPlane(300.0f);
	mCamera.SetPosition({ 0.0f, 23.0f, -33.0f });
	mCamera.SetDirection({ 0.0f,-0.36f, 0.92f });

	Physics::PhysicsWorld::Settings settings;
	settings.drag = 0.3f;
	mPhysicsWorld.Initialize(settings);
	mPhysicsWorld.AddPlane({Vector3::YAxis,0.0f});
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

	if (inputSystem->IsKeyPressed(Input::KeyCode::SPACE))
	{
		
	}

	mPhysicsWorld.Update(deltaTime);
	SimpleDraw::AddGroundPlane(50,false);
}

void GameState::Render()
{
	DrawScene();
}

void GameState::DebugUI()
{
	ImGui::Begin("Physics", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %.2f", Angazi::Core::TimeUtil::GetFramesPerSecond());
	if (ImGui::Button("Particles"))
	{
		mPhysicsWorld.Clear(true);
		for (int i = 0; i < 100; i++)
		{
			auto p = new Physics::Particle;
			p->SetPosition({ RandomFloat(-3.0f,3.0f) , 10.0f , RandomFloat(-3.0f,3.0f) });
			p->SetVelocity({ RandomFloat(-0.05f,0.01f) ,RandomFloat(-0.1f,0.5f),RandomFloat(-0.05f,0.05f) });
			p->radius = 0.1f;
			p->invMass = 0.01f;
			p->bounce = 0.7f;
			mPhysicsWorld.AddParticles(p);
		}
	}
	if (ImGui::Button("Sticks"))
	{
		mPhysicsWorld.Clear(true);
		for (int i = 0; i < 50; i++)
		{
			auto p1 = new Physics::Particle();
			auto p2 = new Physics::Particle();
			p1->SetPosition({ RandomFloat(-3.0f,3.0f) , 10.0f , RandomFloat(-3.0f,3.0f) });
			p1->SetVelocity({ RandomFloat(-0.05f,0.01f) ,RandomFloat(-0.1f,0.5f),RandomFloat(-0.05f,0.05f) });
			p1->radius = 0.1f;
			p1->bounce = 0.3f;

			p2->SetPosition({ RandomFloat(-3.0f,3.0f) , 10.0f , RandomFloat(-3.0f,3.0f) });
			p2->SetVelocity({ RandomFloat(-0.05f,0.01f) ,RandomFloat(-0.1f,0.5f),RandomFloat(-0.05f,0.05f) });
			p2->radius = 0.1f;
			p2->bounce = 0.3f;
			mPhysicsWorld.AddParticles(p1);
			mPhysicsWorld.AddParticles(p2);
			auto c = new Physics::Spring(p1,p2);
			mPhysicsWorld.AddConstraint(c);
		}
	}
	ImGui::End();
}

void GameState::DrawScene()
{
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();
	
	mPhysicsWorld.DebugDraw();

	SimpleDraw::Render(mCamera);
}


// For Homework:
// Add the following
// - Tetrahedron
// - Cubes (how many constraints do you need)
// - Ball and Chain
// - Cloth
// - And if you are up for the challenge, build a figure (may need more constraints)
// - Build whatever you want