#pragma once
#include <Angazi/Inc/Angazi.h>

class Enemy :public Angazi::AI::Agent
{
public:
	Enemy(const Enemy&) = delete;
	Enemy(Angazi::AI::AIWorld& world);

	void SetWanderBehavior(bool set) { mSteeringModule->GetBehavior<Angazi::AI::WanderBehavior>("Wander")->SetActive(set); }
	void SetSeekingBehavior(bool set) { mSteeringModule->GetBehavior<Angazi::AI::SeekingBehavior>("Seek")->SetActive(set); }
	void SetFleeingBehavior(bool set) { mSteeringModule->GetBehavior<Angazi::AI::FleeingBehavior>("Flee")->SetActive(set); }
	void SetHidingBehavior(bool set) { mSteeringModule->GetBehavior<Angazi::AI::HidingBehavior>("Hide")->SetActive(set); }
	void SetObstacleAvoidBehavior(bool set) { mSteeringModule->GetBehavior<Angazi::AI::ObstacleAvoidance>("ObstacleAvoid")->SetActive(set); }
	void SetCohesionBehavior(bool set) { mSteeringModule->GetBehavior<Angazi::AI::CohesionBehavior>("Cohesion")->SetActive(set); }
	void SetAlignmentBehavior(bool set) { mSteeringModule->GetBehavior<Angazi::AI::AlignmentBehavior>("Alighment")->SetActive(set); }
	void SetSeperationBehavior(bool set) { mSteeringModule->GetBehavior<Angazi::AI::SeperationBehavior>("Seperation")->SetActive(set); }

	void Load();
	void Update(float deltaTime);
	void Render() const;
	void Unload();

	bool isDebug = true;
private:
	Angazi::Graphics::TextureId mTextureId;

	std::unique_ptr<Angazi::AI::StateMachine<Enemy>> mStateMachine;
	std::unique_ptr<Angazi::AI::SteeringModule> mSteeringModule;

};