#include "Precompiled.h"
#include "TextureUtil.h"

#ifdef ENABLE_DIRECTX11

#include "D3DUtil.h"
#include <DirectXTK/Inc/WICTextureLoader.h>
#include <DirectXTex/DirectXTex/DirectXTexP.h>
#include "RenderTarget.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "MeshBuffer.h"
#include "Graphics/Inc/MeshBuilder.h"
#include "Graphics/Inc/Camera.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Math;

namespace
{
	Math::Matrix4 cubeLookDir[] =
	{
		Matrix4::RotationQuaternion(Quaternion::RotationLookAt({ 1.0f,  0.0f,  0.0f }, { 0.0f, -1.0f,  0.0f })),
		Matrix4::RotationQuaternion(Quaternion::RotationLookAt({-1.0f,  0.0f,  0.0f }, { 0.0f, -1.0f,  0.0f })),
		Matrix4::RotationQuaternion(Quaternion::RotationLookAt({ 0.0f,  1.0f,  0.0f }, { 0.0f,  0.0f, -1.0f })),
		Matrix4::RotationQuaternion(Quaternion::RotationLookAt({ 0.0f, -1.0f,  0.0f }, { 0.0f,  0.0f,  1.0f })),
		Matrix4::RotationQuaternion(Quaternion::RotationLookAt({ 0.0f,  0.0f, -1.0f }, { 0.0f, -1.0f,  0.0f })),
		Matrix4::RotationQuaternion(Quaternion::RotationLookAt({ 0.0f,  0.0f,  1.0f }, { 0.0f, -1.0f,  0.0f }))
	};
}

ID3D11ShaderResourceView * Angazi::Graphics::TextureUtil::CreateCubeTextureFromHDR(const std::filesystem::path & filePath,)
{
	RenderTarget renderTarget;
	VertexShader vertexShader;
	PixelShader pixelShader;
	MeshBuffer meshBuffer;
	TypedConstantBuffer<Math::Matrix4> tranformBuffer;
	Camera camera;

	DirectX::ScratchImage image;
	ID3D11ShaderResourceView* hdrShaderResourceView;

	HRESULT hr = DirectX::LoadFromHDRFile(filePath.c_str(), nullptr, image);
	ASSERT(SUCCEEDED(hr), "[Texture] Failed to load texture");
	hr = CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), image.GetMetadata(), &hdrShaderResourceView);
	ASSERT(SUCCEEDED(hr), "[Texture] Failed to create shader resource view");

	renderTarget.Initialize(1024, 1024, RenderTarget::Format::RGBA_F16);
	meshBuffer.Initialize(MeshBuilder::CreateInnerCubeP());
	vertexShader.Initialize("../../Assets/Shaders/Equirectangular.fx", VertexP::Format);
	pixelShader.Initialize("../../Assets/Shaders/Equirectangular.fx");
	tranformBuffer.Initialize();

	camera.SetNearPlane(0.1f);
	camera.SetFarPlane(10.0f);
	camera.SetPosition({ });
	camera.SetDirection({ 0.0f,0.0f ,1.0f });
	camera.SetFov(90.0f * Math::Constants::DegToRad);
	camera.SetAspectRatio(1.0f);

	ID3D11Resource* resource;
	renderTarget.GetShaderResourceViewPointer()->GetResource(&resource);

	D3D11_TEXTURE2D_DESC texElementDesc;
	static_cast<ID3D11Texture2D*>(resource)->GetDesc(&texElementDesc);
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
	ASSERT(SUCCEEDED(hr), "[Texture] Failed to create 2D texture");

	// Copy individual texture elements into texture array.
	D3D11_BOX sourceRegion;
	auto matProj = camera.GetPerspectiveMatrix();
	// Copy Mip Map
	for (UINT x = 0; x < 6; x++)
	{
		auto matView = cubeLookDir[x];

		renderTarget.BeginRender();
		pixelShader.Bind();
		vertexShader.Bind();
		GetContext()->PSSetShaderResources(0, 1, &hdrShaderResourceView);
		tranformBuffer.Set(Math::Transpose(matView * matProj));
		tranformBuffer.BindVS(0);
		meshBuffer.Draw();
		renderTarget.EndRender();
		renderTarget.GetShaderResourceViewPointer()->GetResource(&resource);
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
				texArrayDesc.MipLevels), 0, 0, 0, resource, mipLevel, &sourceRegion);
		}
	}
	SafeRelease(resource);

	// Create a resource view to the texture array.
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	viewDesc.TextureCube.MostDetailedMip = 0;
	viewDesc.TextureCube.MipLevels = texArrayDesc.MipLevels;

	ID3D11ShaderResourceView* retShaderResourceView;
	hr = GetDevice()->CreateShaderResourceView(texArray, &viewDesc, &retShaderResourceView);
	ASSERT(SUCCEEDED(hr), "[Texture] Failed to create cube texture");

	SafeRelease(hdrShaderResourceView);
	tranformBuffer.Terminate();
	meshBuffer.Terminate();
	pixelShader.Terminate();
	vertexShader.Terminate();
	renderTarget.Terminate();

	return retShaderResourceView;
}

#endif