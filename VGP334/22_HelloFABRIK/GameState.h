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
	Angazi::Graphics::Camera mCamera;

	Angazi::Math::Vector3 mTarget;
	std::vector<Angazi::Math::Vector3> mPoints;

};
