#pragma once

#include <Angazi/Inc/Angazi.h>
#include "AsteroidManager.h"
#include "BulletManager.h"

class Ship
{
public:
	void Load();

	void Update(float deltaTime);
	void Render();

	void Spawn();
	void Kill();

	Angazi::Math::Circle GetBound() const { return { mPosition,mRadius*0.5f }; }
	bool IsAlive() const { return mAlive; };



	std::unique_ptr<Angazi::AI::NEAT::NeuralNet> brain;
	float fitness = 0.0f;

private:

	Angazi::Math::Vector2 mPosition;
	Angazi::Math::Vector2 mHeading;
	Angazi::Math::Vector2 mVelocity;

	AsteroidManager mAsteroidManager;
	BulletManager mBulletManager;

	float mRadius = 25.0f;
	float mSpeed = 150.0f;
	float mShootDelay = 0.0f;
	const float maxDistance = 500.0f;

	bool mAlive = false;
	bool isMoving = false;
	bool isShooting = false;
};