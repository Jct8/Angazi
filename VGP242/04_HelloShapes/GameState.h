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
	struct Vertex
	{
		Angazi::Math::Vector3 position;
		Angazi::Graphics::Color color;
	};

	//Add 04_HellowShapes project
	//-copy code from , 03_hellotriangle
	//-Update to use DoSomething.fx shaders
	//-Update vertex to have color as well
	//-Make sure inputlayout is initiailized with correct descriptions

	//-Create multiple meshes and vertex buffers
	//-draw a heart
	//-draw a triforce
	//-draw somehting with more than 5 triangles
	//-Use input check so that you can change betwen them

	std::vector<Vertex> mVertices;
	std::map<std::string, std::vector<Vertex>> mMeshes;

	//gets placed in vram
	ID3D11Buffer* mVertexBuffer = nullptr;
	ID3D11VertexShader *mVertexShader = nullptr;
	ID3D11InputLayout *mInputLayout = nullptr;
	ID3D11PixelShader *mPixelShader = nullptr;

	std::vector<ID3D11Buffer*> mVertexBufferList;
	int mCurrentVertexCount = 0;
};
