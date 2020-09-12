#pragma once
#include <Angazi/Inc/Angazi.h>

class VisualSensor;

class SCV : public Angazi::AI::Agent
{
public:
	SCV(Angazi::AI::AIWorld& world);

	void Initialize();

	void Update(float deltaTime);
	void Render();

private:
	std::unique_ptr<Angazi::AI::PerceptionModule> mPerceptionModule;
	VisualSensor* mVisualSensor = nullptr;

	std::unique_ptr<Angazi::AI::SteeringModule> mSteeringModule;
	Angazi::AI::SeekingBehavior* mSeekBehavior = nullptr;
	Angazi::AI::WanderBehavior* mWanderBehavior = nullptr;

	Angazi::Graphics::TextureId mTextureIds[16];
};
