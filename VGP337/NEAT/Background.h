#pragma once

#include <Angazi/Inc/Angazi.h>

class Background
{
public:
	void Load();

	void Update(float deltaTime);
	void Render();

private:
	Angazi::Graphics::TextureId mTextureId;
	Angazi::Math::Vector2 mPosition;
	Angazi::Math::Vector2 mOffset;
};