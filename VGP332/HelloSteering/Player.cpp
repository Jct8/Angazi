#include "Player.h"
#include "ImGui/Inc/imgui.h"
#include "Enemy.h"
#include "TypeId.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void Player::Load()
{
	mTextureId = TextureManager::Get()->Load("../../Assets/Images/XEngine/survivor_handgun.png");
	maxSpeed = 300.0f;
	position = {GraphicsSystem::Get()->GetBackBufferWidth()*0.5f , GraphicsSystem::Get()->GetBackBufferHeight() *0.5f };

	mStateMachine = std::make_unique<AI::StateMachine<Player>>(*this);
	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);

	mSteeringModule->AddBehavior<AI::SeekingBehavior>("Seeking");
	mSteeringModule->AddBehavior<AI::FleeingBehavior>("Fleeing");
	mSteeringModule->AddBehavior<AI::ArriveBehavior>("Arrive");
	mSteeringModule->AddBehavior<AI::WanderBehavior>("Wander");
	mSteeringModule->AddBehavior<AI::PursuitBehavior>("Pursuit");
	//mSteeringModule->GetBehavior<AI::PursuitBehavior>("Pursuit")->evader = &enemy;
	mSteeringModule->AddBehavior<AI::EvadeBehavior>("Evade");
	//mSteeringModule->GetBehavior<AI::EvadeBehavior>("Evade")->pursuer = &enemy;
	mSteeringModule->AddBehavior<AI::HidingBehavior>("Hide");
	mSteeringModule->AddBehavior<AI::ObstacleAvoidance>("ObstacleAvoid");
}

void Player::Update(float deltaTime)
{
	//mStateMachine->Update(deltaTime);
	auto input = Input::InputSystem::Get();
	auto graphics = GraphicsSystem::Get();

	if (input->IsMousePressed(Input::MouseButton::LBUTTON))
	{
		float xPos = static_cast<float>(input->GetMouseScreenX());
		float yPos = static_cast<float>(input->GetMouseScreenY());
		position = Math::Vector2{ xPos +1.0f,yPos + 1.0f };
	}
	if (input->IsKeyPressed(Input::KeyCode::SPACE))
	{
		velocity = { 0.0f,0.0f };
		isPaused = !isPaused;
	}
	if (!isPaused)
	{
		float xPos = static_cast<float>(input->GetMouseScreenX());
		float yPos = static_cast<float>(input->GetMouseScreenY());
		destination = Math::Vector2{ xPos,yPos };

		Math::Vector2 force = mSteeringModule->Calculate();
		if (isDebug)
			mSteeringModule->DebugDraw();

		Math::Vector2 acceleration = force / mass;
		velocity += acceleration * deltaTime;
		auto speed = Math::Magnitude(velocity);
		if (speed > maxSpeed)
		{
			velocity = velocity / speed * maxSpeed;
		}
		position += velocity * deltaTime;

		if (speed > 0.0f)
		{
			heading = Math::Normalize(velocity);
		}

		auto width  = static_cast<float>(graphics->GetBackBufferWidth());
		auto height = static_cast<float>(graphics->GetBackBufferHeight());
		if (position.x < 0.0f)
			position.x += width;
		if (position.x >= width)
			position.x -= width;
		if (position.y < 0.0f)
			position.y += height;
		if (position.y >= height)
			position.y -= height;
	}
}

void Player::Render() const
{
	float angle = atan2(heading.y, heading.x);
	BatchRender::Get()->AddSprite(mTextureId, position, angle);
}

void Player::Unload()
{
	mTextureId = 0;
	mStateMachine.reset();
}

Player::Player(AI::AIWorld& world)
	:Agent(world, static_cast<std::underlying_type_t<TypeId>>(TypeId::Player))
{

}

void Player::DebugUI()
{
	ImGui::Begin("Options", nullptr, ImGuiWindowFlags_NoResize);
	ImGui::BeginGroup();
	if (ImGui::CollapsingHeader("Behavior"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		static bool isSeeking = false;
		static bool isFleeing = false;
		static bool isArriving = false;
		static bool isWandering = false;
		static bool isPursuing = false;
		static bool isEvading = false;
		static bool isHiding = false;
		static bool isAvoidObstacles = false;
		if (ImGui::Checkbox("Seeking ", &isSeeking))
		{
			mSteeringModule->GetBehavior<AI::SeekingBehavior>("Seeking")->SetActive(isSeeking);
		}
		if (ImGui::Checkbox("Fleeing", &isFleeing))
		{
			mSteeringModule->GetBehavior<AI::FleeingBehavior>("Fleeing")->SetActive(isFleeing);
		}
		if (ImGui::Checkbox("Arrive", &isArriving))
		{
			mSteeringModule->GetBehavior<AI::ArriveBehavior>("Arrive")->SetActive(isArriving);
		}
		if (ImGui::Checkbox("Wander", &isWandering))
		{
			mSteeringModule->GetBehavior<AI::WanderBehavior>("Wander")->SetActive(isWandering);
		}
		if (ImGui::Checkbox("Pursuit", &isPursuing))
		{
			mSteeringModule->GetBehavior<AI::PursuitBehavior>("Pursuit")->SetActive(isPursuing);
		}
		if (ImGui::Checkbox("Evade", &isEvading))
		{
			mSteeringModule->GetBehavior<AI::EvadeBehavior>("Evade")->SetActive(isEvading);
		}
		if (ImGui::Checkbox("Hide", &isHiding))
		{
			mSteeringModule->GetBehavior<AI::HidingBehavior>("Hide")->SetActive(isHiding);
		}
		if (ImGui::Checkbox("Avoid Obstacles", &isAvoidObstacles))
		{
			mSteeringModule->GetBehavior<AI::ObstacleAvoidance>("ObstacleAvoid")->SetActive(isAvoidObstacles);
		}
		if (ImGui::Checkbox("Paused", &isPaused))
		{
			velocity = { 0.0f,0.0f };
		}
	}
	ImGui::EndGroup();

	ImGui::BeginGroup();
	if (ImGui::CollapsingHeader("Wander Parameters"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::SliderFloat("Wander Radius", &mSteeringModule->GetBehavior<AI::WanderBehavior>("Wander")->wanderRadius, 0.0f, 300.0f);
		ImGui::SliderFloat("Wander Distance", &mSteeringModule->GetBehavior<AI::WanderBehavior>("Wander")->wanderDistance, 0.0f, 300.0f);
		ImGui::SliderFloat("Wander Jitter", &mSteeringModule->GetBehavior<AI::WanderBehavior>("Wander")->wanderJitter, 0.1f, 50.0f);
	}
	ImGui::EndGroup();

	ImGui::BeginGroup();
	if (ImGui::CollapsingHeader("Obstacles Avoidance Parameters"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::SliderFloat("Lateral Force Scale", &mSteeringModule->GetBehavior<AI::ObstacleAvoidance>("ObstacleAvoid")->lateralForceScale, 0.0f, 10.0f);
		ImGui::SliderFloat("Braking Weight", &mSteeringModule->GetBehavior<AI::ObstacleAvoidance>("ObstacleAvoid")->brakingWeight, 0.0f, 5.0f);
		ImGui::SliderFloat("Box size x", &mSteeringModule->GetBehavior<AI::ObstacleAvoidance>("ObstacleAvoid")->boxExtend.x, 1.0f, 50.0f);
		ImGui::SliderFloat("Box size y", &mSteeringModule->GetBehavior<AI::ObstacleAvoidance>("ObstacleAvoid")->boxExtend.y, 1.0f, 500.0f);
	}
	ImGui::EndGroup();

	ImGui::BeginGroup();
	if (ImGui::CollapsingHeader("Speed Parameters"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::SliderFloat("Player Speed", &maxSpeed, 0.0f, 1000.0f);
	}
	ImGui::EndGroup();

	ImGui::BeginGroup();
	if (ImGui::CollapsingHeader("Debug Options"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::Checkbox("Enable Player Debug", &isDebug);
		//ImGui::Checkbox("Enable Enmey Debug", &enemy.isDebug);
	}
	ImGui::EndGroup();

	ImGui::End();

}

