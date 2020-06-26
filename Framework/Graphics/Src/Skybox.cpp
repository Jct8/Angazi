#include "Precompiled.h"
#include "Skybox.h"

#include "Camera.h"
#include "MeshBuilder.h"
#include "VertexTypes.h"

using namespace Angazi;
using namespace Angazi::Graphics;

Skybox::Skybox()
{
	//cubeSides[Right] = "../../Assets/Images/Skybox/right.jpg";
	//cubeSides[Left] = "../../Assets/Images/Skybox/left.jpg";
	//cubeSides[Top] = "../../Assets/Images/Skybox/top.jpg";
	//cubeSides[Bottom] = "../../Assets/Images/Skybox/bottom.jpg";
	//cubeSides[Front] = "../../Assets/Images/Skybox/front.jpg";
	//cubeSides[Back] = "../../Assets/Images/Skybox/back.jpg";

	cubeSides[Right] = "../../Assets/Images/Skybox2/right.jpg";
	cubeSides[Left] = "../../Assets/Images/Skybox2/left.jpg";
	cubeSides[Top] = "../../Assets/Images/Skybox2/top.jpg";
	cubeSides[Bottom] = "../../Assets/Images/Skybox2/bottom.jpg";
	cubeSides[Front] = "../../Assets/Images/Skybox2/front.jpg";
	cubeSides[Back] = "../../Assets/Images/Skybox2/back.jpg";

	//cubeSides[Right] = "../../Assets/Images/Skybox3/right.png";
	//cubeSides[Left] = "../../Assets/Images/Skybox3/left.png";
	//cubeSides[Top] = "../../Assets/Images/Skybox3/top.png";
	//cubeSides[Bottom] = "../../Assets/Images/Skybox3/bottom.png";
	//cubeSides[Front] = "../../Assets/Images/Skybox3/front.png";
	//cubeSides[Back] = "../../Assets/Images/Skybox3/back.png";

	//cubeSides[Right] = "../../Assets/Images/Skybox4/right.png";
	//cubeSides[Left] = "../../Assets/Images/Skybox4/left.png";
	//cubeSides[Top] = "../../Assets/Images/Skybox4/top.png";
	//cubeSides[Bottom] = "../../Assets/Images/Skybox4/bottom.png";
	//cubeSides[Front] = "../../Assets/Images/Skybox4/front.png";
	//cubeSides[Back] = "../../Assets/Images/Skybox4/back.png";

	//cubeSides[Right] = "../../Assets/Images/Skybox5/right.png";
	//cubeSides[Left] = "../../Assets/Images/Skybox5/left.png";
	//cubeSides[Top] = "../../Assets/Images/Skybox5/top.png";
	//cubeSides[Bottom] = "../../Assets/Images/Skybox5/bottom.png";
	//cubeSides[Front] = "../../Assets/Images/Skybox5/front.png";
	//cubeSides[Back] = "../../Assets/Images/Skybox5/back.png";

	//cubeSides[Right] = "../../Assets/Images/Skybox6/right.png";
	//cubeSides[Left] = "../../Assets/Images/Skybox6/left.png";
	//cubeSides[Top] = "../../Assets/Images/Skybox6/top.png";
	//cubeSides[Bottom] = "../../Assets/Images/Skybox6/bottom.png";
	//cubeSides[Front] = "../../Assets/Images/Skybox6/front.png";
	//cubeSides[Back] = "../../Assets/Images/Skybox6/back.png";

	//cubeSides[Right] = "../../Assets/Images/Skybox7/right.png";
	//cubeSides[Left] = "../../Assets/Images/Skybox7/left.png";
	//cubeSides[Top] = "../../Assets/Images/Skybox7/top.png";
	//cubeSides[Bottom] = "../../Assets/Images/Skybox7/bottom.png";
	//cubeSides[Front] = "../../Assets/Images/Skybox7/front.png";
	//cubeSides[Back] = "../../Assets/Images/Skybox7/back.png";
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
	mDepthStencilState.Initialize(true, true);

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