#pragma once

#include <XEngine.h>

class Projectile
{
public:
	void Load(std::string fileName);
	void Unload();
	void Update(float deltaTime);
	void Render();

	void Fire(bool isPlayer,float speed, int damage, X::Math::Vector2 targetPos, X::Math::Vector2 startingPos);

	bool IsActive() { return isActive; }
private:
	X::Math::Vector2 mPosition;
	X::Math::Vector2 mStartingPosition;
	X::Math::Vector2 mDirection;
	X::TextureId mTexture;
	X::TextureId mBloodTexture = 0;

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

