#pragma once
#include <Angazi/Inc/Angazi.h>

class Player :public Angazi::AI::Agent
{
public:
	Player(Angazi::AI::AIWorld& world);
	Player(const Player&) = delete;

	void Load();
	void Update(float deltaTime);
	void Render() const;
	void Unload();
	void DebugUI();

private:
	Angazi::Graphics::TextureId mTextureId;

	std::unique_ptr<Angazi::AI::StateMachine<Player>> mStateMachine;
	std::unique_ptr<Angazi::AI::SteeringModule> mSteeringModule;
	std::unique_ptr<Angazi::AI::PerceptionModule> mPerceptionModule;

	bool isPaused = false;
	bool isDebug = true;
};