#include "Player.h"
#include "Enemy.h"
#include "ImGui/Inc/imgui.h"
#include "TypeId.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void Player::Load()
{
	mTextureId = TextureManager::Get()->Load("../../Assets/Images/XEngine/survivor_handgun.png");
	maxSpeed = 300.0f;
	position = { GraphicsSystem::Get()->GetBackBufferWidth()* 0.5f , GraphicsSystem::Get()->GetBackBufferHeight() * 0.5f };

	AI::ImportanceCalculator importanceCalculator =
		[](const Agent& agent, AI::MemoryRecord& record)
	{
		record.importance = Math::DistanceSqr(agent.position, std::get<Math::Vector2>(record.properties["lastSeenPosition"]));
	};
	mPerceptionModule = std::make_unique<AI::PerceptionModule>(*this, importanceCalculator);
	mPerceptionModule->AddSensor<AI::VisualSensor>("VisualSensor");
	mPerceptionModule->GetSensor<AI::VisualSensor>("VisualSensor")->viewAngle = 90;
	mPerceptionModule->GetSensor<AI::VisualSensor>("VisualSensor")->viewRange = 150;
	mPerceptionModule->SetMemorySpan(5.0f);

	mStateMachine = std::make_unique<AI::StateMachine<Player>>(*this);
	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);

	mSteeringModule->AddBehavior<AI::ArriveBehavior>("Arrive")->SetActive(true);;
}

Player::Player(AI::AIWorld& world)
	:Agent(world, static_cast<std::underlying_type_t<TypeId>>(TypeId::Player))
{

}

void Player::Update(float deltaTime)
{
	auto input = Input::InputSystem::Get();
	auto graphics = GraphicsSystem::Get();

	mPerceptionModule->Update(deltaTime);

	AI::MemoryRecords memoryRecords = mPerceptionModule->GetMemoryRecords();

	if (input->IsMousePressed(Input::MouseButton::LBUTTON))
	{
		float xPos = static_cast<float>(input->GetMouseScreenX());
		float yPos = static_cast<float>(input->GetMouseScreenY());
		position = Math::Vector2{ xPos + 1.0f,yPos + 1.0f };
	}
	if (input->IsKeyPressed(Input::KeyCode::SPACE))
	{
		velocity = { 0.0f,0.0f };
		isPaused = !isPaused;
	}

	if (!memoryRecords.empty())
	{
		destination = std::get<Math::Vector2>(memoryRecords.back().properties["lastSeenPosition"]);
	}
	else
	{
		velocity = { 0.0f,0.0f };
		return;
	}
	if (!isPaused)
	{
		SimpleDraw::AddScreenCircle(destination, 5.0f, Colors::AliceBlue);
		float xPos = static_cast<float>(input->GetMouseScreenX());
		float yPos = static_cast<float>(input->GetMouseScreenY());

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

		auto width = static_cast<float>( graphics->GetBackBufferWidth());
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

void Player::DebugUI()
{
	ImGui::Begin("Options", nullptr);
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
	}
	ImGui::EndGroup();

	ImGui::End();

}

