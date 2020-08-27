#pragma once
#include <Angazi/Inc/Angazi.h>

class Particle
{
public:
	void Load(std::string  fileName, int columns, int rows);
	void Unload();
	void Update(float deltaTime);
	void Render();

	void Start(const Angazi::Math::Vector2 &pos);
	bool IsActive() { return isActive; }
	void SetPivot(Angazi::Graphics::Pivot pivot) { mPivot = pivot; }

	int GetTextureWidth() { return Angazi::Graphics::TextureManager::Get()->GetTexture(mTexture)->GetWidth(); }

private:
	Angazi::Graphics::TextureId mTexture;
	Angazi::Math::Vector2 mPosition;

	int mFrame = 0;
	int mFrameCount = 0;

	float mDuration = 0.0f;
	float mFrameTimer = 0.0f;

	bool isActive = false;
	int mColumns;
	int mRows;

	Angazi::Graphics::Pivot mPivot = Angazi::Graphics::Pivot::Center;

};

