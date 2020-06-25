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
	stbi_uc* imageData = stbi_load(filePath.u8string().c_str(), &width, &height, &channels, STBI_rgb_alpha);

	ASSERT(imageData, "[TextureGL] - Failed to load image!");
	mWidth = width;
	mHeight = height;
	GLenum internalFormat = GL_RGBA8;
	GLenum dataFormat = GL_RGBA;

	glCreateTextures(GL_TEXTURE_2D, 1, &mTextureID);
	glTextureStorage2D(mTextureID, 1, internalFormat, mWidth, mHeight);
	glTextureSubImage2D(mTextureID, 0, 0, 0, mWidth, mHeight, dataFormat, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);
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