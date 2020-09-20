#pragma once
#include <Angazi/Inc/Angazi.h>

class EditState : public Angazi::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;

	void DebugUI() override;

private:
	void RenderScene();
	void ShowSceneView();

private:

	Angazi::Graphics::HdrEffect mHdrEffect;
	Angazi::Graphics::RenderTarget mRenderTarget;

	Angazi::GameWorld mWorld;
	Angazi::Editor mEditor{ mWorld };
	Angazi::CameraService* mCameraService;
	Angazi::EnvironmentService* mEnvironmentService;
};
