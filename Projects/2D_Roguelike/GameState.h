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

public:
	void RunGameStart();
	void RunGamePlay(float deltaTime);
	void RunGameEnd();

private:
	Angazi::Graphics::Camera mCamera;

	Angazi::Graphics::TextureId backgroundTex;
	Angazi::Graphics::TextureId characterTex;
	Angazi::Graphics::TextureId character2Tex;
	Angazi::Math::Vector2 currentView;

	float cameraOffset;

};

//TODO-Angazi:
// Sound effects
// Falling attack
// Add wall slide/jump

//TODO-Angazi Later:
// Fix attack crash
// Add more weapons
// Controller support
// Add different particles
// Add wall climb
// Enemy Attack Delay
// Deflect bullets