#include "Precompiled.h"
#include "Skybox.h"

#include "Camera.h"
#include "MeshBuilder.h"
#include "VertexTypes.h"

using namespace Angazi;
using namespace Angazi::Graphics;

Skybox::Skybox()
{
	cubeSides[Right] = "../../Assets/Images/Skybox/right.jpg";
	cubeSides[Left] = "../../Assets/Images/Skybox/left.jpg";
	cubeSides[Top] = "../../Assets/Images/Skybox/top.jpg";
	cubeSides[Bottom] = "../../Assets/Images/Skybox/bottom.jpg";
	cubeSides[Front] = "../../Assets/Images/Skybox/front.jpg";
	cubeSides[Back] = "../../Assets/Images/Skybox/back.jpg";
}

Skybox::~Skybox()
{
}

void Skybox::AddTexture(const std::filesystem::path & path, Side side)
{
	cubeSides[side] = path;
}

void Skybox::CreateSkybox()
{
	//Texture
	std::vector<std::filesystem::path> fileNames;
	for (std::map<Side, std::filesystem::path>::iterator it = cubeSides.begin(); it != cubeSides.end(); ++it) 
		fileNames.push_back(it->second);
	mTexture.Initialize(fileNames);

	// Render States
	mRasterizerState.Initialize(RasterizerState::CullMode::None, RasterizerState::FillMode::Solid);
	mDepthStencilState.Initialize(true,true);

	//MeshBuffer
	mBoxBuffer.Initialize(MeshBuilder::CreateInnerCubeP());

	// Shaders
#ifdef ENABLE_DIRECTX11
	mVertexShader.Initialize("../../Assets/Shaders/Skybox.fx", VertexP::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Skybox.fx");
#endif
#ifdef ENABLE_OPENGL
	mVertexShader.Initialize("../../Assets/GLShaders/Skybox.glsl", VertexP::Format);
	mPixelShader.Initialize("../../Assets/GLShaders/Skybox.glsl");
#endif
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
	mTransformBuffer.Initialize();
}

void Skybox::Terminate()
{
	mTransformBuffer.Terminate();
	mSampler.Terminate();

	mPixelShader.Terminate();
	mVertexShader.Terminate();

	mBoxBuffer.Terminate();
	mDepthStencilState.Clear();
	mRasterizerState.Terminate();

	mTexture.Terminate();
}

void Skybox::Draw(const Angazi::Graphics::Camera & camera)
{
	// Sampler
	mSampler.BindPS();
	// Bind Texture
	mTexture.BindPS(0);

	auto matView = camera.GetViewMatrix();
	auto matProj = camera.GetPerspectiveMatrix();
	matView._41 = 0.0f;
	matView._42 = 0.0f;
	matView._43 = 0.0f;

	mTransformData.wvp = Transpose(matView * matProj);
	mTransformBuffer.Set(mTransformData);
	mTransformBuffer.BindVS(0);

	mDepthStencilState.Set();
	mRasterizerState.Set();

	mVertexShader.Bind();
	mPixelShader.Bind();

	mBoxBuffer.Draw();
	mRasterizerState.Clear();
}