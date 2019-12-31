#include "Precompiled.h"
#include "TextureGL.h"
#include "GraphicsGL/stb_image/stb_image.h"

using namespace Angazi;
using namespace Angazi::GraphicsGL;

void TextureGL::Initialize(const std::filesystem::path filePath, bool flip)
{
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	int width, height, bytesPerPixel;
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* imageData = stbi_load(filePath.u8string().c_str(), &width, &height, &bytesPerPixel,0);

	if (imageData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB , GL_UNSIGNED_BYTE,imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(imageData);
	}
}

void TextureGL::Terminate()
{
	glDeleteTextures(1, &mTextureID);
}

void TextureGL::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}