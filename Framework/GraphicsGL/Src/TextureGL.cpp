#include "Precompiled.h"
#include "TextureGL.h"

#ifdef ENABLE_OPENGL

#include "stb_image.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void Texture::Initialize(const std::filesystem::path& filePath, bool gammaCorrection)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(false);
	stbi_uc* imageData = stbi_load(filePath.u8string().c_str(), &width, &height, &channels, 0);

	ASSERT(imageData, "[TextureGL] - Failed to load image!");
	mWidth = width;
	mHeight = height;

	GLenum internalFormat = 0, dataFormat = 0;
	if (channels == 4)
	{
		internalFormat = gammaCorrection ? GL_SRGB8_ALPHA8 : GL_RGBA8;
		dataFormat = GL_RGBA;
	}
	else if (channels == 3)
	{
		internalFormat = gammaCorrection ? GL_SRGB8 : GL_RGB8;
		dataFormat = GL_RGB;
	}
	else if (channels == 2)
	{
		internalFormat = GL_RG8;
		dataFormat = GL_RG;
	}
	else if (channels == 1)
	{
		internalFormat = GL_R8;
		dataFormat = GL_RED;
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &mTextureID);
	glTextureStorage2D(mTextureID, 1, internalFormat, mWidth, mHeight);
	glTextureSubImage2D(mTextureID, 0, 0, 0, mWidth, mHeight, dataFormat, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);
}

void Texture::Initialize(const std::vector<std::filesystem::path>& cubeSides, bool gammaCorrection)
{
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);

	int width, height, nrChannels;
	GLenum internalFormat = gammaCorrection ? GL_SRGB8_ALPHA8 : GL_RGBA8;
	for (size_t i = 0; i < cubeSides.size(); i++)
	{
		stbi_uc *data = stbi_load(cubeSides[i].u8string().c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			ASSERT(false, "[TextureGL] - Cubemap texture failed to load path");
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Texture::InitializeHdrCube(const std::filesystem::path& filePath, const std::filesystem::path& shaderFilePath, uint32_t cubeLength)
{
	ASSERT(false, "[TextureL] Not implemented for OpenGL.");

	//DirectX::ScratchImage image;
	//ID3D11ShaderResourceView* hdrShaderResourceView;

	//HRESULT hr = DirectX::LoadFromHDRFile(filePath.c_str(), nullptr, image);
	//ASSERT(SUCCEEDED(hr), "[Texture] Failed to load texture");
	//hr = CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), image.GetMetadata(), &hdrShaderResourceView);
	//ASSERT(SUCCEEDED(hr), "[Texture] Failed to create shader resource view");

	//mShaderResourceView = TextureUtil::CreateCubeMapFromTexture(hdrShaderResourceView, shaderFilePath, cubeLength);

	//SafeRelease(hdrShaderResourceView);

	//// Get Width and Height of the texture
	//ID3D11Resource* resource = nullptr;
	//mShaderResourceView->GetResource(&resource);

	//ID3D11Texture2D* texture = static_cast<ID3D11Texture2D*>(resource);
	//D3D11_TEXTURE2D_DESC desc{};
	//texture->GetDesc(&desc);
	//SafeRelease(resource);

	//mWidth = desc.Width;
	//mHeight = desc.Height;
}

void Texture::InitializeCubeMap(Texture& texture, const std::filesystem::path& shaderFilePath, uint32_t cubeLength, CubeMapType type)
{
	ASSERT(false, "[TextureGL] Not implemented for OpenGL.");

	//switch (type)
	//{
	//case Angazi::Graphics::Texture::IrradianceMap:
	//	mShaderResourceView = TextureUtil::CreateCubeMapFromTexture(texture.GetShaderResourceView(), shaderFilePath, cubeLength);
	//	break;
	//case Angazi::Graphics::Texture::PreFiltered:
	//	mShaderResourceView = TextureUtil::CreatePreFilteredCubeMap(texture.GetShaderResourceView(), shaderFilePath, cubeLength);
	//	break;
	//default:
	//	break;
	//}

	//// Get Width and Height of the texture
	//ID3D11Resource* resource = nullptr;
	//mShaderResourceView->GetResource(&resource);

	//ID3D11Texture2D* tex = static_cast<ID3D11Texture2D*>(resource);
	//D3D11_TEXTURE2D_DESC desc{};
	//tex->GetDesc(&desc);
	//SafeRelease(resource);

	//mWidth = desc.Width;
	//mHeight = desc.Height;
}


Texture::~Texture()
{
	ASSERT(!glIsTexture(mTextureID), "[TextureGL] Terminate() must be called to clean up!");
}

void Texture::Terminate()
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::BindVS(uint32_t slot) const
{
	glBindTextureUnit(slot, mTextureID);
}

void Texture::BindPS(uint32_t slot) const
{
	glBindTextureUnit(slot, mTextureID);
}
void Texture::UnbindVS(uint32_t slot) const
{
	glBindTextureUnit(slot, mTextureID);
}
void Texture::UnbindPS(uint32_t slot) const
{
	glBindTextureUnit(slot, mTextureID);
}

#endif