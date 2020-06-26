#include "Precompiled.h"
#include "TextureGL.h"

#ifdef ENABLE_OPENGL

#include "stb_image.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void Texture::Initialize(const std::filesystem::path& filePath)
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
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	}
	else if (channels == 3)
	{
		internalFormat = GL_RGB8;
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

void Texture::Initialize(const std::vector<std::filesystem::path>& cubeSides)
{
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);

	int width, height, nrChannels;
	for (size_t i = 0; i < cubeSides.size(); i++)
	{
		stbi_uc *data = stbi_load(cubeSides[i].u8string().c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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