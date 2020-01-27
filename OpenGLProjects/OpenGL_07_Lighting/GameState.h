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
	Angazi::GraphicsGL::CameraGL mCamera;

	Angazi::GraphicsGL::Mesh mMesh;
	Angazi::GraphicsGL::MeshBufferGL mMeshBuffer;

	Angazi::GraphicsGL::DirectionalLight mDirectionalLight;
	Angazi::GraphicsGL::Material mMaterial;

	struct TransformData
	{
		Angazi::Math::Matrix4 world;
		Angazi::Math::Matrix4 wvp;
		Angazi::Math::Vector3 viewPosition;
		float padding;
	};

	Angazi::GraphicsGL::Shader mGouraudShadingShader;

	Angazi::GraphicsGL::Shader mPhongShadingShader;


	Angazi::Math::Vector3 mRotation = 0.0f;
};
