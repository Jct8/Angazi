#include "Precompiled.h"
#include "TextureGL.h"

#ifdef ENABLE_OPENGL

#include "stb_image.h"
#include "TextureGLUtil.h"

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
		stbi_uc* data = stbi_load(cubeSides[i].u8string().c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
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
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	float* data = stbi_loadf(filePath.u8string().c_str(), &width, &height, &channels, 0);
	ASSERT(data != nullptr, "TextureGL -- Failed to load hdr image.");

	uint32_t hdrTexture;
	glGenTextures(1, &hdrTexture);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);

	mTextureID = TextureUtil::CreateCubeMapFromTexture(hdrTexture, shaderFilePath, cubeLength);
	glDeleteTextures(1, &hdrTexture);

	mWidth = width;
	mHeight = height;
}

void Texture::InitializeCubeMap(const Texture& texture, const std::filesystem::path& shaderFilePath, uint32_t cubeLength, CubeMapType type)
{

	switch (type)
	{
	case Angazi::Graphics::Texture::IrradianceMap:
		mTextureID = TextureUtil::CreateCubeMapFromTexture(texture.GetTextureID(), shaderFilePath, cubeLength);
		break;
	case Angazi::Graphics::Texture::PreFiltered:
		mTextureID = TextureUtil::CreatePreFilteredCubeMap(texture.GetTextureID(), shaderFilePath, cubeLength);
		break;
	default:
		break;
	}

	mWidth = texture.GetWidth();
	mHeight = texture.GetHeight();
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