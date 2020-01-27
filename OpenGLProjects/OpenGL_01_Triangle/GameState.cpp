#include "GameState.h"

using namespace Angazi;
using namespace Angazi::GraphicsGL;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{
	mMeshPC.vertices.push_back({ Vector3{  0.5f, -0.5f, 0.0f } , Colors::Gold });
	mMeshPC.vertices.push_back({ Vector3{ -0.5f, -0.5f, 0.0f } , Colors::Purple });
	mMeshPC.vertices.push_back({ Vector3{  0.0f,  0.5f, 0.0f } , Colors::White });

	mMeshPC.indices.push_back(0);
	mMeshPC.indices.push_back(1);
	mMeshPC.indices.push_back(2);

	mMeshBuffer.Initialize(mMeshPC, VertexPC::Format);

	mShader.Initialize("../../Assets/GLShaders/GLColor.glsl");
	mShader.Bind();

}

void GameState::Terminate()
{
	mShader.Terminate();
	mMeshBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{

}

void GameState::Render()
{
	mShader.Bind();
	mMeshBuffer.Draw();
}