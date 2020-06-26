#include "GameState.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Gray);
	mMeshPX.vertices.push_back({ Vector3{  0.5f,  0.5f, 0.0f } , Vector2{1.0f, 1.0f }});
	mMeshPX.vertices.push_back({ Vector3{  0.5f, -0.5f, 0.0f } , Vector2{1.0f, 0.0f }});
	mMeshPX.vertices.push_back({ Vector3{ -0.5f, -0.5f, 0.0f } , Vector2{0.0f, 0.0f }});
	mMeshPX.vertices.push_back({ Vector3{ -0.5f,  0.5f, 0.0f } , Vector2{0.0f, 1.0f }});

	mMeshPX.indices.push_back(0);
	mMeshPX.indices.push_back(3);
	mMeshPX.indices.push_back(1);

	mMeshPX.indices.push_back(1);
	mMeshPX.indices.push_back(3);
	mMeshPX.indices.push_back(2);

	mMeshBuffer.Initialize(mMeshPX);

	mVertexShader.Initialize("../../Assets/GLShaders/DoTexturing.glsl", VertexPX::Format);
	mPixelShader.Initialize("../../Assets/GLShaders/DoTexturing.glsl");
	mTexture.Initialize("../../Assets/Images/Goat.jpg");
	mTransformBuffer.Initialize();

}

void GameState::Terminate()
{
	mTransformBuffer.Terminate();

	mTexture.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{


}

void GameState::Render()
{
	data.wvp = Matrix4::Identity;
	mTransformBuffer.Set(data);
	mTransformBuffer.BindVS(0);
	mVertexShader.Bind();
	mPixelShader.Bind();
	mTexture.BindPS(0);
	mMeshBuffer.Draw();
}