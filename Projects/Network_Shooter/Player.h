#pragma once

#include "Bullet.h"

class Player
{
public:
	void Initialize(int playerNumber, float speed, const char* playerPath, const char* healthBarPath);
	void Terminate();

	void Update(float deltaTime);
	void Render();

public:
	void ModifyHealth(int health);
	void Shoot();
	void Reset();

	Angazi::Math::Circle GetCollider() { return { mPosition, Angazi::Graphics::TextureManager::Get()->GetTexture(mTextureId)->GetWidth() * 0.25f }; }
	Angazi::Math::Vector2 GetPosition() { return mPosition; };
	Angazi::Math::Vector2 GetHeading() { return mHeading; };

	void SetPosition(Angazi::Math::Vector2 pos) { mPosition = pos; }
	void SetHeading(Angazi::Math::Vector2 heading) { mHeading = heading; }

	// Flags
	bool GetMoveState() { return mMoved; }
	bool GetShotState() { return mShot; }
	bool IsActive() { return isActive; }
	bool IsAlive() { return mHealth > 0; }

	void SetMoveState(bool move) { mMoved = move; }
	void SetShotState(bool shot) { mShot = shot; }
	void SetSelectedPlayer(bool selected) { isSelected = selected; }
	void SetActive(bool active) { isActive = active; }

private:
	float UpdateAngle();
	void CheckWallCollisions();

	std::array<Bullet,50> mBullets;

	Angazi::Math::Vector2 mPosition = 0.0f;
	Angazi::Math::Vector2 mHeading = 0.0f;

	Angazi::Graphics::TextureId mTextureId = 0;
	Angazi::Graphics::TextureId healthBarEmpty = 0;
	Angazi::Graphics::TextureId healthBarFull = 0;

	float mSpeed = 200.0f;
	int mPlayerNumber = 0;
	int mHealth = 10;
	const int mMaxHealth = 10;

	bool mMoved = false;
	bool mShot = false;
	bool isSelected = false;
	bool isActive = false;
};