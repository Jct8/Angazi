#include "..\..\VGP337\AsteroidsNEAT\Bullet.h"
#include "Bullet.h"
#include "Player.h"

using namespace Angazi;
using namespace Angazi::Graphics;

extern std::vector<Angazi::Math::LineSegment> walls;
extern std::array<Player, 4> players;

void Bullet::Initialize(int playerNumber)
{
	mTextureId = TextureManager::Get()->Load("Network/bullet2.png");
	mPlayerNumber = playerNumber;
	isActive = false;
}

void Bullet::Terminate()
{
	isActive = false;
	mTextureId = 0;
}

void Bullet::Update(float deltaTime)
{
	if (!isActive)
		return;
	mPosition += mVelocity * mSpeed * deltaTime;
	
	// Wall Collisons
	if (mPosition.x < 0.0f)
		isActive = false;
	if (mPosition.x > GraphicsSystem::Get()->GetBackBufferWidth())
		isActive = false;
	if (mPosition.y < 0.0f)
		isActive = false;
	if (mPosition.y > GraphicsSystem::Get()->GetBackBufferHeight())
		isActive = false;

	for (auto& wall : walls)
	{
		if (Math::Intersect(wall, GetCollider()))
			isActive = false;
	}

	// Check player collisions
	for (size_t i = 0; i < players.size(); i++)
	{
		if (mPlayerNumber != i && players[i].IsAlive() && players[i].IsActive() && Angazi::Math::Intersect(players[i].GetCollider(), GetCollider()))
		{
			isActive = false;
			players[i].ModifyHealth(-mDamage);
		}
	}
}

void Bullet::Render()
{
	if (isActive)
		BatchRender::Get()->AddSprite(mTextureId, mPosition);
}
