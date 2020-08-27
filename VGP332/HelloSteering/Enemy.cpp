#include "Enemy.h"
#include "TypeId.h"

using namespace Angazi;
using namespace Angazi::Graphics;

Enemy::Enemy(AI::AIWorld& world)
	:Agent(world, static_cast<std::underlying_type_t<TypeId>>(TypeId::Enemy))
{

}

void Enemy::Load()
{
	mTextureId = TextureManager::Get()->Load("../../Assets/Images/XEngine/interceptor_09.png");
	maxSpeed = 500.0f;
	position = { GraphicsSystem::Get()->GetBackBufferWidth() *0.3f, GraphicsSystem::Get()->GetBackBufferHeight() *0.3f};

	mStateMachine = std::make_unique<AI::StateMachine<Enemy>>(*this);
	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);

	mSteeringModule->AddBehavior<AI::WanderBehavior>("Wander");
	mSteeringModule->GetBehavior<AI::WanderBehavior>("Wander")->SetActive(false);
	mSteeringModule->AddBehavior<AI::SeekingBehavior>("Seek");
	mSteeringModule->GetBehavior<AI::SeekingBehavior>("Seek")->SetActive(false);
	mSteeringModule->AddBehavior<AI::FleeingBehavior>("Flee");
	mSteeringModule->GetBehavior<AI::FleeingBehavior>("Flee")->SetActive(false);
	mSteeringModule->AddBehavior<AI::HidingBehavior>("Hide");
	mSteeringModule->GetBehavior<AI::HidingBehavior>("Hide")->SetActive(false);
	mSteeringModule->AddBehavior<AI::ObstacleAvoidance>("ObstacleAvoid");
	mSteeringModule->GetBehavior<AI::ObstacleAvoidance>("ObstacleAvoid")->SetActive(false);
	mSteeringModule->AddBehavior<AI::CohesionBehavior>("Cohesion")->SetActive(false);
	mSteeringModule->AddBehavior<AI::AlignmentBehavior>("Alighment")->SetActive(false);
	mSteeringModule->AddBehavior<AI::SeperationBehavior>("Seperation")->SetActive(false);
}

void Enemy::Update(float deltaTime)
{
	auto input = Input::InputSystem::Get();
	auto graphics = GraphicsSystem::Get();

	//Update neightbors (exclude self)
	neighbors = world.GetNeighborhood({ position,100.0f }, static_cast<std::underlying_type_t<TypeId>>(TypeId::Enemy));
	auto endIter = std::remove_if(neighbors.begin(), neighbors.end(), [this](auto neighbor){return this == neighbor;});

	float xPos = static_cast<float>(input->GetMouseScreenX());
	float yPos = static_cast<float>(input->GetMouseScreenY());
	destination = threat->position;

	if (input->IsMousePressed(Input::MouseButton::RBUTTON))
		position = Math::Vector2{ xPos,yPos };

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

	auto width = static_cast<float>(graphics->GetBackBufferWidth());
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

void Enemy::Render() const
{
	float angle = atan2(heading.y, heading.x);
	BatchRender::Get()->AddSprite(mTextureId, position, angle);
}

void Enemy::Unload()
{
	mTextureId = 0;
}