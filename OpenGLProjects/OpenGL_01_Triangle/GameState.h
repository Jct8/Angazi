#pragma once
#include <Angazi/Inc/Angazi.h>

class GameState : public Angazi::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;

private:
	Angazi::Graphics::Shader mShader;
	Angazi::Graphics::MeshBufferGL mMeshBuffer;

	Angazi::Graphics::MeshPC mMeshPC;
};