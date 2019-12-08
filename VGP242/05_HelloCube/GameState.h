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

	//H0omework
	//Move to graphics library
	struct Vertex
	{
		Angazi::Math::Vector3 position;
		Angazi::Graphics::Color color;
	};
	//Add a mesh Struct
	std::vector<Vertex> mVertices;
	std::vector<uint32_t> mIndices;

	//add constant buffer class to wrap this
	// - Initialize(int size)/Terminate()
	// - Bind()
	// - Set(void* data)
	ID3D11Buffer* mConstantBuffer = nullptr;
	
	//add meshbuffer class for these pointers
	// - Initialize(Vertex * vertices, int vertexCount, uint32_t* indices, int indexCount)
	// - Terminate()
	// - Draw()
	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11Buffer* mIndexBuffer = nullptr;

	//Just do these in HelloCube, add a few more cubes please

	Angazi::Graphics::VertexShader mVertexShader;
	Angazi::Graphics::PixelShader mPixelShader;


	float mRotation = 0.0f;

	

};
