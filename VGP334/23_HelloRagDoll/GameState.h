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
	void DrawScene();
	void PostProcess();

private:
	Angazi::Graphics::Camera mCamera;

	// Light
	Angazi::Graphics::DirectionalLight mDirectionalLight;
	Angazi::Graphics::Material mMaterial;

	// Physics
	Angazi::Physics::PhysicsWorld mPhysicsWorld;
	

	// Model
	Angazi::Graphics::Model mModel;
	Angazi::Graphics::Animator animator;
	Angazi::Graphics::StandardEffect mModelStandardEffect;

	bool mShowSkeleton = false;
	bool mApplyPhysics = false;

};
