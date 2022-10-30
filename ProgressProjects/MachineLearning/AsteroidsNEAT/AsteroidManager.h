#pragma once

#include "Asteroid.h"
#include "BulletManager.h"

class Ship;

class AsteroidManager
{
public:
	void Update(float deltaTime, BulletManager& bulletManager, Ship& ship);
	void Render();

	void Reset();

	bool Intersect(const Angazi::Math::Circle& circle) const;
	float Intersect(const Angazi::Math::LineSegment& line, float maxDistance) const;

private:
	void SpawnAsteroid(const Angazi::Math::Vector2& position, Asteroid::Type type);

	std::array<Asteroid,100> mAsteroids;
	float mSpawnDelay = 0.0f;
};

