#include "GameState.h"

using namespace Angazi;
using namespace Angazi::GraphicsGL;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{
	mMeshPC.vertices.push_back({ Vector3{  0.5f,  0.5f, 0.0f } , 1.0f, 1.0f });
	mMeshPC.vertices.push_back({ Vector3{  0.5f, -0.5f, 0.0f } , 1.0f, 0.0f });
	mMeshPC.vertices.push_back({ Vector3{ -0.5f, -0.5f, 0.0f } , 0.0f, 0.0f });
	mMeshPC.vertices.push_back({ Vector3{ -0.5f,  0.5f, 0.0f } , 0.0f, 1.0f });

	mMeshPC.indices.push_back(0);
	mMeshPC.indices.push_back(1);
	mMeshPC.indices.push_back(3);

	mMeshPC.indices.push_back(1);
	mMeshPC.indices.push_back(2);
	mMeshPC.indices.push_back(3);

	mMeshBuffer.Initialize(mMeshPC, VertexPX::Format);

	mShader.Initialize("../../Assets/Shaders/BasicTexturing.glsl");
	mShader.Bind();

	mTexture.Initialize("Goat.jpg");
}

void GameState::Terminate()
{
	mTexture.Terminate();
	mShader.Terminate();
	mMeshBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{

}

void GameState::Render()
{
	mTexture.Bind();
	mShader.Bind();
	mMeshBuffer.Draw();
}