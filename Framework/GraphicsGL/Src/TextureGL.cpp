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
	stbi_uc* imageData = stbi_load(filePath.u8string().c_str(), &width, &height, &channels, STBI_rgb);

	ASSERT(imageData, "[TextureGL] - Failed to load image!");
	mWidth = width;
	mHeight = height;
	GLenum internalFormat = GL_RGB8;
	GLenum dataFormat = GL_RGB;


	glCreateTextures(GL_TEXTURE_2D, 1, &mTextureID);
	glTextureStorage2D(mTextureID, 1, internalFormat, mWidth, mHeight);

	glTextureParameteri(mTextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(mTextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTextureParameteri(mTextureID, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTextureParameteri(mTextureID, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTextureSubImage2D(mTextureID, 0, 0, 0, mWidth, mHeight, dataFormat, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);
}

Texture::~Texture()
{

}

void Texture::Terminate()
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::BindVS(uint32_t slot) const
{
	//glActiveTexture(GL_TEXTURE0 + slot);
	//glBindTexture(GL_TEXTURE_2D, mTextureID);
	glBindTextureUnit(slot, mTextureID);
}

void Texture::BindPS(uint32_t slot) const
{
	//glActiveTexture(GL_TEXTURE0 + slot);
	//glBindTexture(GL_TEXTURE_2D, mTextureID);
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

//void TextureGL::Bind(const std::string & name, unsigned int slot) const
//{
//	Bind(slot);
//	glUniform1i(glGetUniformLocation(mTextureID, name.c_str()), slot);
//}


#endif