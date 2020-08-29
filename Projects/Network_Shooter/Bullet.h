#pragma once
#include <Angazi/Inc/Angazi.h>

class Bullet
{
public:
	void Initialize(int playerNumber);
	void Terminate();

	void Update(float deltaTime);
	void Render();
public:
	void SetPosition(Angazi::Math::Vector2 position) { mPosition = position; }
	void SetVelocity(Angazi::Math::Vector2 velocity) { mVelocity = Angazi::Math::Normalize(velocity); }
	void SetActive(bool active) { isActive = active; }

	Angazi::Math::Circle GetCollider() { return { mPosition, Angazi::Graphics::TextureManager::Get()->GetTexture(mTextureId)->GetWidth() * 0.5f }; }
	bool IsActive() { return isActive; }

private:

	Angazi::Math::Vector2 mPosition = 0.0f;
	Angazi::Math::Vector2 mVelocity = 0.0f;

	Angazi::Graphics::TextureId mTextureId = 0;

	float mSpeed = 1200.0f;
	int mPlayerNumber = 0;
	int mDamage = 1;
	bool isActive = false;
};