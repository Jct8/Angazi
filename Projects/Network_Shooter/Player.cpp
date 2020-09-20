#include "Player.h"

extern int totalPlayers;

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;

namespace
{
	std::vector<Angazi::Math::Vector2> healthBarPositions =
	{
		{ 25.0f, 25.0f },
		{ 1000.0f ,680.0f },
		{ 1000.0f , 25.0f },
		{ 25.0f, 680.0f }
	};

	std::vector<Angazi::Math::Vector2> playerTextPositions =
	{
		{ 65.0f  , 75.0f },
		{ 1040.0f , 630.0f },
		{ 1040.0f, 75.0f },
		{ 65.0f  , 630.0f }
	};

	std::vector<Angazi::Math::Vector2> spawnPositions =
	{
		{ 100.0f, 120.0f },
		{ 1100.0f ,630.0f } ,
		{ 1100.0f , 120.0f },
		{ 100.0f, 630.0f }
	};
}

void Player::Initialize(int playerNumber, float speed, const char* playerPath, const char* healthBarPath)
{
	mTextureId = TextureManager::Get()->Load(playerPath);
	healthBarEmpty = TextureManager::Get()->Load("Network/bar_empty.png");
	healthBarFull = TextureManager::Get()->Load(healthBarPath);

	mPlayerNumber = playerNumber;
	mPosition = spawnPositions[playerNumber];
	mSpeed = speed;
	mHealth = mMaxHealth;

	for (auto& bullet : mBullets)
		bullet.Initialize(playerNumber);
}

void Player::Terminate()
{
	mTextureId = 0;
	healthBarEmpty = 0;
	healthBarFull = 0;

	for (auto& bullet : mBullets)
		bullet.Terminate();
}

void Player::Update(float deltaTime)
{
	if (mHealth <= 0)
		return;

	auto inputSystem = InputSystem::Get();

	if (isSelected)
	{
		if (inputSystem->IsKeyDown(KeyCode::W))
		{
			mPosition.y -= deltaTime * mSpeed;
			mMoved = true;
		}
		if (inputSystem->IsKeyDown(KeyCode::S))
		{
			mPosition.y += deltaTime * mSpeed;
			mMoved = true;
		}
		if (inputSystem->IsKeyDown(KeyCode::A))
		{
			mPosition.x -= deltaTime * mSpeed;
			mMoved = true;
		}
		if (inputSystem->IsKeyDown(KeyCode::D))
		{
			mPosition.x += deltaTime * mSpeed;
			mMoved = true;
		}

		CheckWallCollisions();

		if (inputSystem->GetMouseMoveX() != 0 || inputSystem->GetMouseMoveY() != 0)
		{
			Angazi::Math::Vector2 targetPosition = { static_cast<float>(inputSystem->GetMouseScreenX()),static_cast<float>(inputSystem->GetMouseScreenY()) };
			mHeading = Angazi::Math::Normalize(targetPosition - mPosition);
			mMoved = true;
		}

		if (inputSystem->IsMousePressed(MouseButton::LBUTTON))
		{
			Shoot();
			mMoved = true;
			mShot = true;
		}
	}

	for (auto& bullet : mBullets)
		bullet.Update(deltaTime);
}

void Player::Render()
{
	// Player Text
	std::string playerText = "Player " + std::to_string(mPlayerNumber + 1);
	BatchRenderer::Get()->AddScreenText(playerText.c_str(), playerTextPositions[mPlayerNumber].x, playerTextPositions[mPlayerNumber].y, 30.0f, Colors::AliceBlue);

	if (isSelected)
	{
		playerText = "You are Player " + std::to_string(mPlayerNumber + 1);
		BatchRenderer::Get()->AddScreenText(playerText.c_str(), GraphicsSystem::Get()->GetBackBufferWidth() * 0.5f - 150.0f, 25.0f, 30.0f, Colors::AliceBlue);
	}

	// Health bar Render
	auto position = healthBarPositions[mPlayerNumber];
	float playerHealthPercent = static_cast<float>(mHealth) / mMaxHealth;

	Math::Rect rect
	{
		0.0f,0.0f,
		playerHealthPercent * TextureManager::Get()->GetTexture(healthBarFull)->GetWidth(),
		1.0f * TextureManager::Get()->GetTexture(healthBarFull)->GetHeight()
	};
	BatchRenderer::Get()->AddSprite(healthBarEmpty, position, Pivot::TopLeft);
	if (playerHealthPercent > 0.0f)
		BatchRenderer::Get()->AddSprite(healthBarFull, rect, position, Pivot::TopLeft, Flip::None);

	if (mHealth <= 0)
		return;

	// Player Render
	float angle = UpdateAngle();
	BatchRenderer::Get()->AddSprite(mTextureId, mPosition, angle);

	// Bullets
	for (auto& bullet : mBullets)
		bullet.Render();
}

void Player::ModifyHealth(int health)
{
	if (mHealth <= 0)
		return;

	mHealth += health;
	if (mHealth <= 0)
	{
		mHealth = 0;
		totalPlayers--;
	}
}

float Player::UpdateAngle()
{
	Math::Vector2 direction = Math::Normalize(mHeading);
	return atan2(direction.y, direction.x);
}

void Player::CheckWallCollisions()
{
	// Wall Collisons
	float spriteWidth = TextureManager::Get()->GetTexture(mTextureId)->GetWidth() * 0.25f;
	float spriteHeight = TextureManager::Get()->GetTexture(mTextureId)->GetHeight() *0.25f;

	if (mPosition.x - spriteWidth < 0.0f)
		mPosition.x = spriteWidth;
	if (mPosition.x + spriteWidth > GraphicsSystem::Get()->GetBackBufferWidth())
		mPosition.x = GraphicsSystem::Get()->GetBackBufferWidth() - spriteWidth;
	if (mPosition.y - spriteHeight < 0.0f)
		mPosition.y = spriteHeight;
	if (mPosition.y + spriteHeight > GraphicsSystem::Get()->GetBackBufferHeight())
		mPosition.y = GraphicsSystem::Get()->GetBackBufferHeight() - spriteHeight;
}

void Player::Shoot()
{
	for (auto& bullet : mBullets)
	{
		if (!bullet.IsActive())
		{
			bullet.SetActive(true);
			bullet.SetVelocity(mHeading);
			bullet.SetPosition(mPosition + mHeading * (TextureManager::Get()->GetTexture(mTextureId)->GetWidth() * 0.25f));
			break;
		}
	}
}

void Player::Reset()
{
	mPosition = spawnPositions[mPlayerNumber];
	mHealth = mMaxHealth;

	mMoved = false;
	mShot = false;
	isSelected = false;
	isActive = false;

	for (auto& bullet : mBullets)
		bullet.SetActive(false);
}
