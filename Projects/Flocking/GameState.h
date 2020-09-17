#pragma once
#include <Angazi/Inc/Angazi.h>
#include "Enemy.h"

class GameState : public Angazi::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	
	void DebugUI() override;
	void EnemyDebugUI();

private:
	Angazi::Graphics::Camera mCamera;

	Angazi::AI::AIWorld mAIWorld;
	Angazi::AI::AIWorld::Settings mAISettings;

	const int maxEneimes = 200;
	std::vector<std::unique_ptr<Enemy>> enemies;
};
