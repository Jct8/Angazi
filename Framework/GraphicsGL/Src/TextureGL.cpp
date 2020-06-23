#include "Precompiled.h"
#include "TextureGL.h"

#ifdef ENABLE_OPENGL

#include "stb_image.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void Texture::Initialize(const std::filesystem::path& filePath)
{
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	int width, height, bytesPerPixel;
	stbi_set_flip_vertically_on_load(false);
	unsigned char* imageData = stbi_load(filePath.u8string().c_str(), &width, &height, &bytesPerPixel,0);

	if (imageData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB , GL_UNSIGNED_BYTE,imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(imageData);
	}
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