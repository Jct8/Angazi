#pragma once
#include <Angazi/Inc/Angazi.h>

class GameState : public Angazi::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;

	void DebugUI() override;

private:

	void RestartAlgorithm();
	Angazi::Graphics::Camera mCamera;
	std::vector<Angazi::Math::Vector2> mCities;

	Angazi::AI::GeneticAlgorithm mGeneticAlgorithm;

	float threshold;
	float mCurrentDelay = 0.0f;
	float mDelay = 0.0f;
	float mutationRate = 0.4f;
	int numCities = 20;
	int mPopulation = 100;
};
