#pragma once
#include <Angazi/Inc/Angazi.h>

class GameState : public Angazi::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltatTime) override;
	void Render() override;

private:
	Angazi::Graphics::Camera mCamera;

	struct Vertex
	{
		Angazi::Math::Vector3 position;
		Angazi::Graphics::Color color;
	};

	std::vector<Vertex> mVertices;

	ID3D11Buffer* mConstantBuffer = nullptr;
	ID3D11Buffer* mVertexBuffer = nullptr;

	Angazi::Graphics::VertexShader mVertexShader;
	Angazi::Graphics::PixelShader mPixelShader;


	float mRotation = 0.0f;

};
