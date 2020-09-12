#include "SCV.h"

#include "TypeIds.h"
#include "VisualSensor.h"

using namespace Angazi;
using namespace Angazi::Graphics;

extern float viewRange;
extern float viewAngle;

extern float wanderRadius;
extern float wanderDistance;
extern float wanderJitter;

namespace
{
	float ComputeImportance(const AI::Agent& agent, const AI::MemoryRecord& record)
	{
		float distanceScore = 0.0f;
		float resourceScore = 0.0f;

		auto iter = record.properties.find("lastSeenLocation");
		if (iter != record.properties.end())
		{
			auto& property = iter->second;
			auto position = std::get<Math::Vector2>(property);
			distanceScore = 5000.0f - Math::Distance(agent.position, position);
		}

		iter = record.properties.find("resourceAmount");
		if (iter != record.properties.end())
		{
			auto& property = iter->second;
			resourceScore = std::get<float>(property);
		}

		return resourceScore + distanceScore;
	};
}

SCV::SCV(AI::AIWorld& world)
	: AI::Agent(world, TypeIds::SCV)
{
}

void SCV::Initialize()
{
	maxSpeed = 200.0f;

	mPerceptionModule = std::make_unique<AI::PerceptionModule>(*this, ComputeImportance);
	mVisualSensor = mPerceptionModule->AddSensor<VisualSensor>("VisualSensor");

	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
	mSeekBehavior = mSteeringModule->AddBehavior<AI::SeekingBehavior>("SeekBehavior");
	mSeekBehavior->SetActive(false);
	mWanderBehavior = mSteeringModule->AddBehavior<AI::WanderBehavior>("WanderBehavior");
	mWanderBehavior->SetActive(true);

	for (size_t i = 0; i < std::size(mTextureIds); ++i)
	{
		char name[256];
		sprintf_s(name, "XEngine/scv_%02zu.png", i + 1);
		mTextureIds[i] = TextureManager::Get()->Load(name);
	}
}

void SCV::Update(float deltaTime)
{
	mVisualSensor->viewRange = viewRange;
	mVisualSensor->viewAngle = viewAngle * Math::Constants::DegToRad;
	mWanderBehavior->wanderRadius = wanderRadius;
	mWanderBehavior->wanderDistance = wanderDistance;
	mWanderBehavior->wanderJitter = wanderJitter;

	mPerceptionModule->Update(deltaTime);

	auto& memory = mPerceptionModule->GetMemoryRecords();
	if (!memory.empty())
	{
		for (auto iter = memory.begin(); iter != memory.end(); ++iter)
		{
			const AI::MemoryRecord& m = (*iter);
			auto property = m.properties.find("lastSeenLocation");
			auto pos = std::get<Math::Vector2>(property->second);

			if (iter == memory.begin())
			{
				// Do something about it!
				//mWanderBehavior->SetActive(false);
				//mSeekBehavior->SetActive(true);
				//destination = pos;
			}

			SimpleDraw::AddScreenLine(position, pos, Colors::Beige);

			std::string score;
			score += std::to_string(m.importance);
			BatchRender::Get()->AddScreenText(score.c_str(), pos.x, pos.y, 12.0f, Colors::White);
		}
	}
	else
	{
		//mWanderBehavior->SetActive(true);
		//mSeekBehavior->SetActive(false);
	}

	auto force = mSteeringModule->Calculate();
	auto acceleration = force / mass;
	velocity += acceleration * deltaTime;
	auto speedSqr = Math::MagnitudeSqr(velocity);
	if (speedSqr > Math::Sqr(maxSpeed))
		velocity = Math::Normalize(velocity) * maxSpeed;
	position += velocity * deltaTime;

	if (auto posToDest = destination - position;
		Math::MagnitudeSqr(posToDest) < 250.0f)
		velocity = Math::RandomUnitCircle() * maxSpeed * 0.75f;

	if (Math::MagnitudeSqr(velocity) > 1.0f)
		heading = Math::Normalize(velocity);

	auto screenWidth = GraphicsSystem::Get()->GetBackBufferWidth();
	auto screenHeight = GraphicsSystem::Get()->GetBackBufferHeight();
	if (position.x < 0.0f)
		position.x += screenWidth;
	if (position.x >= screenWidth)
		position.x -= screenWidth;
	if (position.y < 0.0f)
		position.y += screenHeight;
	if (position.y >= screenHeight)
		position.y -= screenHeight;
}

void SCV::Render()
{
	float angle = atan2(-heading.x, heading.y) + Math::Constants::Pi;
	int frame = (int)(angle / Math::Constants::TwoPi * std::size(mTextureIds)) % std::size(mTextureIds);
	BatchRender::Get()->AddSprite(mTextureIds[frame], position);
}