#pragma once

#include "Bullet.h"

class BulletManager
{
public:
	void Update(float deltaTime);
	void Render();

	void Reset();

	bool Intersect(const Angazi::Math::Circle& circle) const;
	void FireBullet(const Angazi::Math::Vector2 & velocity, const Angazi::Math::Vector2& position);

	std::array<Bullet, 20>& GetBullets()  { return mBullets; }

private:
	std::array<Bullet, 20> mBullets;
	float mSpawnDelay = 0.0f;
};

