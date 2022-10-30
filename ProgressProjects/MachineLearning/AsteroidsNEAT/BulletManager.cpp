#include "BulletManager.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void BulletManager::Update(float deltaTime)
{
	for (auto& bullet : mBullets)
		bullet.Update(deltaTime);
}

void BulletManager::Render()
{
	for (auto& bullet : mBullets)
		bullet.Render();
}

void BulletManager::Reset()
{
	for (auto& bullet : mBullets)
		bullet.SetActive(false);
}

bool BulletManager::Intersect(const Angazi::Math::Circle& circle) const
{
	/*for (auto& bullet : mBullets)
	{
		if (bullet.IsActive() && Math::Intersect(bullet.GetBoundingCircle(), circle))
			return true;
	}*/
	return false;
}

void BulletManager::FireBullet(const Math::Vector2& velocity, const Math::Vector2& position)
{
	for (auto& bullet : mBullets)
	{
		if (!bullet.IsActive())
		{
			bullet.Fire(velocity, position);
			break;
		}
	}
}

