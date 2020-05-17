#include "Precompiled.h"
#include "Texture.h"

#include "D3DUtil.h"
#include <DirectXTK/Inc/WICTextureLoader.h>

using namespace Angazi::Graphics;

Texture::~Texture()
{
	ASSERT(mShaderResourceView == nullptr, "[Texture] Terminate() must be called to clean up!");
}

void Texture::Initialize(const std::filesystem::path& fileName)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(GetDevice(), GetContext(), fileName.c_str(), nullptr, &mShaderResourceView);
	ASSERT(SUCCEEDED(hr), "Failed to load texture %ls.", fileName.c_str());

	// Get Width and Height of the texture
	ID3D11Resource* resource = nullptr;
	mShaderResourceView->GetResource(&resource);
	
	ID3D11Texture2D* texture = static_cast<ID3D11Texture2D*>(resource);
	D3D11_TEXTURE2D_DESC desc{};
	texture->GetDesc(&desc);

	mWidth = desc.Width;
	mHeight = desc.Height;
}

void Texture::Terminate()
{
	SafeRelease(mShaderResourceView);
}

void Texture::BindVS(uint32_t slot) const
{
	GetContext()->VSSetShaderResources(slot, 1, &mShaderResourceView);
}

void Texture::BindPS(uint32_t slot) const
{
	GetContext()->PSSetShaderResources(slot, 1, &mShaderResourceView);
}

void Texture::UnbindVS(uint32_t slot) const
{
	static ID3D11ShaderResourceView* dummy = nullptr;
	GetContext()->VSSetShaderResources(slot, 1, &dummy);
}

void Texture::UnbindPS(uint32_t slot) const
{
	static ID3D11ShaderResourceView* dummy = nullptr;
	GetContext()->PSSetShaderResources(slot, 1, &dummy);
}
