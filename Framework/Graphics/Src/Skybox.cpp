#include "Precompiled.h"
#include "Skybox.h"

#include "D3DUtil.h"
#include <DirectXTK/Inc/WICTextureLoader.h>
//#include "RasterizerState.h"
#include "MeshBuilder.h"


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
	ASSERT(mShaderResourceView == nullptr, "[Skybox] Terminate() must be called to clean up!");
}

void Skybox::AddTexture(const std::filesystem::path & path, Side side)
{
	cubeSides[side] = path;
}

void Skybox::CreateSkybox()
{
	HRESULT hr;
	std::array<ID3D11Resource*, 6> resourceArray;
	std::array<ID3D11ShaderResourceView*,6> srvArray;
	int i = 0;
	for (auto side : cubeSides)
	{
		hr = DirectX::CreateWICTextureFromFile(GetDevice(), GetContext(), side.second.c_str(), nullptr, &srvArray[i]);
		ASSERT(SUCCEEDED(hr), "[Skybox] Failed to load texture %ls.", side.second.c_str());
		srvArray[i]->GetResource(&resourceArray[i]);
		i++;
	}

	D3D11_TEXTURE2D_DESC texElementDesc;
	static_cast<ID3D11Texture2D*>(resourceArray[0])->GetDesc(&texElementDesc);

	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = texElementDesc.Width;
	texArrayDesc.Height = texElementDesc.Height;
	texArrayDesc.MipLevels = texElementDesc.MipLevels;
	texArrayDesc.ArraySize = 6;
	texArrayDesc.Format = texElementDesc.Format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	ID3D11Texture2D* texArray = 0;
	hr = GetDevice()->CreateTexture2D(&texArrayDesc, 0, &texArray);
	ASSERT(SUCCEEDED(hr), "[Skybox] Failed to create 2D texture");

	// Copy individual texture elements into texture array.
	D3D11_BOX sourceRegion;
	// Copy Mip Map
	for (UINT x = 0; x < 6; x++)
	{
		for (UINT mipLevel = 0; mipLevel < texArrayDesc.MipLevels; mipLevel++)
		{
			sourceRegion.left = 0;
			sourceRegion.right = (texArrayDesc.Width >> mipLevel);
			sourceRegion.top = 0;
			sourceRegion.bottom = (texArrayDesc.Height >> mipLevel);
			sourceRegion.front = 0;
			sourceRegion.back = 1;

			//test for overflow
			if (sourceRegion.bottom == 0 || sourceRegion.right == 0)
				break;

			GetContext()->CopySubresourceRegion(texArray, D3D11CalcSubresource(mipLevel, x,
				texArrayDesc.MipLevels), 0, 0, 0, resourceArray[x], mipLevel, &sourceRegion);
		}
		SafeRelease(resourceArray[x]);
		SafeRelease(srvArray[x]);
	}

	// Create a resource view to the texture array.
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	viewDesc.TextureCube.MostDetailedMip = 0;
	viewDesc.TextureCube.MipLevels = texArrayDesc.MipLevels;

	hr = GetDevice()->CreateShaderResourceView(texArray, &viewDesc, &mShaderResourceView);
	ASSERT(SUCCEEDED(hr),"[Skybox] Failed to create cube texture");

	// Render States
	mRasterizerState.Initialize(RasterizerState::CullMode::None, RasterizerState::FillMode::Solid);
	mDepthStencilState.Initialize(true);

	//MeshBuffer
	mBoxBuffer.Initialize(MeshBuilder::CreateInnerCubeP());
	//mBoxBuffer.Initialize(MeshBuilder::CreatePlanePX( 100, 100));

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
	mDepthStencilState.Clear();
	mRasterizerState.Terminate();

	SafeRelease(mShaderResourceView);
}

void Skybox::Draw(Math::Matrix4 wvp)
{
	// Sampler
	mSampler.BindPS();
	// Bind Texture
	GetContext()->PSSetShaderResources(0, 1, &mShaderResourceView);

	mTransformData.wvp = wvp;
	mTransformBuffer.Set(mTransformData);
	mTransformBuffer.BindVS(0);

	mDepthStencilState.Set();
	mRasterizerState.Set();

	mVertexShader.Bind();
	mPixelShader.Bind();

	mBoxBuffer.Draw();
	mRasterizerState.Clear();
}
