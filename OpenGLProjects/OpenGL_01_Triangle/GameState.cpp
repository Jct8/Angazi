#include "GameState.h"

using namespace Angazi;
using namespace Angazi::Graphics;
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

	mMeshBuffer.Initialize(mMeshPC);

	mVertexShader.Initialize("../../Assets/GLShaders/DoSomething.glsl", VertexPC::Format);
	mPixelShader.Initialize("../../Assets/GLShaders/DoSomething.glsl");
}

void GameState::Terminate()
{
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{

}

void GameState::Render()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mMeshBuffer.Draw();

}