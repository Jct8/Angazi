#include "AsteroidManager.h"

#include "Ship.h"

using namespace Angazi;
using namespace Angazi::Graphics;


void AsteroidManager::Update(float deltaTime, BulletManager& bulletManager, Ship& ship)
{
	float width = (float)GraphicsSystem::Get()->GetBackBufferHeight();
	float height = (float)GraphicsSystem::Get()->GetBackBufferHeight();
	mSpawnDelay -= deltaTime;
	if (mSpawnDelay <= 0.0f)
	{
		int randomType = Math::RandomInt(0, Asteroid::Type::End - 1);
		auto position = Math::RandomVector2({ 0.0f,0.0 }, { width,height });

		Math::Vector2 pos;
		auto randomInt = Math::RandomInt(0, 1);

		if (randomInt == 0)
		{
			pos.x = Math::RandomFloat(0.0f, width);
			auto randomInt2 = Math::RandomInt(0, 1);
			if (randomInt2 == 0)
				pos.y = 0.0f;
			else 
				pos.y = height;
		}
		else
		{
			pos.y = Math::RandomFloat(0.0f, width);
			auto randomInt2 = Math::RandomInt(0, 1);
			if (randomInt2 == 0)
				pos.x = 0.0f;
			else
				pos.x = width;
		}

		SpawnAsteroid(pos, Asteroid::Type(randomType));
		mSpawnDelay = 0.2f;
	}

	for (auto& asteroid : mAsteroids)
	{
		asteroid.Update(deltaTime);
		if (!asteroid.IsActive())
			continue;
		for (auto& bullet : bulletManager.GetBullets())
		{
			if (!bullet.IsActive())
				continue;
			if (Math::Intersect(bullet.GetBoundingCircle(), asteroid.GetBoundingCircle()))
			{
				Asteroid::Type type = asteroid.GetType();
				if (type != Asteroid::Type::Small)
				{
					SpawnAsteroid(asteroid.GetPosition(), Asteroid::Type(type - 1));
					SpawnAsteroid(asteroid.GetPosition(), Asteroid::Type(type - 1));
				}
				bullet.SetActive(false);
				asteroid.SetActive(false);
				ship.fitness += 1.0f;
				break;
			}
		}
	}

}

void AsteroidManager::Render()
{
	for (auto& asteroid : mAsteroids)
		asteroid.Render();
}

void AsteroidManager::Reset()
{
	for (auto& asteroid : mAsteroids)
		asteroid.SetActive(false);
	mSpawnDelay = 0.0f;
}

bool AsteroidManager::Intersect(const Angazi::Math::Circle& circle) const
{
	for (auto& asteroid : mAsteroids)
	{
		if (asteroid.IsActive() && Math::Intersect(asteroid.GetBoundingCircle(), circle))
			return true;
	}
	return false;
}

float AsteroidManager::Intersect(const Angazi::Math::LineSegment & line, float maxDistance) const
{
	float closestDistance = maxDistance;
	for (auto& asteroid : mAsteroids)
	{
		Math::Vector2 closestPoint;
		if (asteroid.IsActive() && Math::Intersect(asteroid.GetBoundingCircle(), line, &closestPoint))
		{
			auto distance = Math::Distance(line.from, closestPoint);
			if (distance < closestDistance)
				closestDistance = distance;
		}
	}
	return closestDistance;
}

void AsteroidManager::SpawnAsteroid(const Angazi::Math::Vector2& position, Asteroid::Type type)
{
	float width = (float)GraphicsSystem::Get()->GetBackBufferHeight();
	float height = (float)GraphicsSystem::Get()->GetBackBufferHeight();
	for (auto& asteroid : mAsteroids)
	{
		if (!asteroid.IsActive())
		{
			asteroid.Load();
			asteroid.Spawn(position, type);
			break;
		}
	}
}
