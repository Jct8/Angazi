#include "Precompiled.h"
#include "Skybox.h"

#include "Camera.h"
#include "MeshBuilder.h"
#include "VertexTypes.h"

using namespace Angazi;
using namespace Angazi::Graphics;

Skybox::Skybox()
{
	ChangeDefualtSkybox(1);
}

Skybox::~Skybox()
{
}

void Skybox::AddTexture(const std::filesystem::path & path, Side side)
{
	cubeSides[side] = path;
}

void Skybox::CreateSkybox(const std::filesystem::path& hdrImagePath)
{
	//Texture
	if (hdrImagePath == "None")
	{
		std::vector<std::filesystem::path> fileNames;
		for (std::map<Side, std::filesystem::path>::iterator it = cubeSides.begin(); it != cubeSides.end(); ++it)
			fileNames.push_back(it->second);
		mTexture.Initialize(fileNames, true);
		mIrradianceMap.InitializeCubeMap(mTexture, "../../Assets/Shaders/IrradianceMap.fx", 32, Texture::CubeMapType::IrradianceMap);
		mPrefilterMap.InitializeCubeMap(mTexture, "../../Assets/Shaders/PreFilter.fx", 256, Texture::CubeMapType::PreFiltered);
	}
	else
	{
		mTexture.InitializeHdrCube(hdrImagePath, "../../Assets/Shaders/Equirectangular.fx", 1024);
		mIrradianceMap.InitializeCubeMap(mTexture, "../../Assets/Shaders/IrradianceMap.fx", 32,Texture::CubeMapType::IrradianceMap);
		mPrefilterMap.InitializeCubeMap(mTexture, "../../Assets/Shaders/PreFilter.fx", 256, Texture::CubeMapType::PreFiltered);
	}

	// Render States
#ifdef ENABLE_DIRECTX11
	mRasterizerState.Initialize(RasterizerState::CullMode::Back, RasterizerState::FillMode::Solid);
#endif
#ifdef ENABLE_OPENGL
	mRasterizerState.Initialize(RasterizerState::CullMode::Front, RasterizerState::FillMode::Solid);
#endif
	mDepthStencilState.Initialize(true, false);

	//MeshBuffer
	mBoxBuffer.Initialize(MeshBuilder::CreateInnerCubeP());

	// Shaders
	mVertexShader.Initialize("../../Assets/Shaders/Skybox.fx", VertexP::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Skybox.fx");
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
	mDepthStencilState.Terminate();
	mRasterizerState.Terminate();

	mPrefilterMap.Terminate();
	mIrradianceMap.Terminate();
	mTexture.Terminate();
}

void Skybox::ChangeDefualtSkybox(int skyboxNumber)
{
	std::string defaultPath = "../../Assets/Images/";
	switch (skyboxNumber)
	{
	case 1:	defaultPath.append("Skybox"); break;
	case 2:	defaultPath.append("Skybox2"); break;
	case 3:	defaultPath.append("Skybox3"); break;
	case 4:	defaultPath.append("Skybox4"); break;
	case 5:	defaultPath.append("Skybox5"); break;
	case 6:	defaultPath.append("Skybox6"); break;
	case 7:	defaultPath.append("Skybox7"); break;
	default:defaultPath.append("Skybox"); break;
	}
	cubeSides[Right] = defaultPath + "/right.png";
	cubeSides[Left] = defaultPath + "/left.png";
	cubeSides[Top] = defaultPath + "/top.png";
	cubeSides[Bottom] = defaultPath + "/bottom.png";
	cubeSides[Front] = defaultPath + "/front.png";
	cubeSides[Back] = defaultPath + "/back.png";
}

void Skybox::Draw(const Angazi::Graphics::Camera & camera)
{
	// Sampler
	mSampler.BindPS();
	// Bind Texture
	mTexture.BindPS(0);
	//mIrradianceMap.BindPS(0);
	//mPrefilterMap.BindPS(0);

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
	mDepthStencilState.Clear();
}