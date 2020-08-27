#pragma once
#include <Angazi/Inc/Angazi.h>

class Projectile
{
public:
	void Load(std::string fileName);
	void Unload();
	void Update(float deltaTime);
	void Render();

	void Fire(bool isPlayer,float speed, int damage, Angazi::Math::Vector2 targetPos, Angazi::Math::Vector2 startingPos);

	bool IsActive() { return isActive; }
private:
	Angazi::Math::Vector2 mPosition;
	Angazi::Math::Vector2 mStartingPosition;
	Angazi::Math::Vector2 mDirection;
	Angazi::Graphics::TextureId mTexture;
	Angazi::Graphics::TextureId mBloodTexture;

	int mFrame = 0;
	int mFrameCount = 0;
	float mDuration = 0.0f;
	float mFrameTimer = 0.0f;

	float mSpeed = 10.0f;
	float mGravity = 200.0f;
	float mProjectileFallDist = 300.0f;

	int mDamage = 10;
	bool isActive = false;
	bool isPlayingAnimation = false;
	bool mIsPlayer = false;
};

