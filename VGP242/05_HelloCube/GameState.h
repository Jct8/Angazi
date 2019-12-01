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

	//gets placed in vram
	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11VertexShader *mVertexShader = nullptr;
	ID3D11InputLayout *mInputLayout = nullptr;
	ID3D11PixelShader *mPixelShader = nullptr;

	float mRotation = 0.0f;

};
