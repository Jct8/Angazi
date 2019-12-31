#include "GameState.h"

using namespace Angazi;
using namespace Angazi::GraphicsGL;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{
	mMeshPX.vertices.push_back({ Vector3{  0.5f,  0.5f, 0.0f } , 1.0f, 1.0f });
	mMeshPX.vertices.push_back({ Vector3{  0.5f, -0.5f, 0.0f } , 1.0f, 0.0f });
	mMeshPX.vertices.push_back({ Vector3{ -0.5f, -0.5f, 0.0f } , 0.0f, 0.0f });
	mMeshPX.vertices.push_back({ Vector3{ -0.5f,  0.5f, 0.0f } , 0.0f, 1.0f });

	mMeshPX.indices.push_back(0);
	mMeshPX.indices.push_back(1);
	mMeshPX.indices.push_back(3);

	mMeshPX.indices.push_back(1);
	mMeshPX.indices.push_back(2);
	mMeshPX.indices.push_back(3);

	mMeshBuffer.Initialize(mMeshPX, VertexPX::Format);

	mShader.Initialize("../../Assets/Shaders/GLTexturing.glsl");
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