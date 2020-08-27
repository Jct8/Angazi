#include "Survivor.h"

#include "EnemyManager.h"
#include "Handgun.h"
#include "Rifle.h"
#include "Shotgun.h"

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	Angazi::Math::Vector2 GetTargetPosition()
	{
		Angazi::Math::Vector2 mousePosition;
		mousePosition.x = static_cast<float>(Input::InputSystem::Get()->GetMouseScreenX());
		mousePosition.y = static_cast<float>(Input::InputSystem::Get()->GetMouseScreenY());
		return mousePosition;
	}
}

void Survivor::Load()
{
	mWeapons[0] = new Handgun();
	mWeapons[1] = new Rifle();
	mWeapons[2] = new Shotgun();

	for (auto & w : mWeapons)
	{
		w->Load();
	}

	//Select first weapon by default
	mCurrentWeapon = mWeapons[0];
}

void Survivor::Unload()
{
	mCurrentWeapon = nullptr;
	for (auto &w : mWeapons)
	{
		w->Unload();
		delete w;
	}
	
}

void Survivor::Update(float deltaTime)
{
	auto input = Input::InputSystem::Get();
	if (!isAlive)
		return;

	if (input->IsKeyPressed(Input::KeyCode::ONE))
		mCurrentWeapon = mWeapons[0];
	if (input->IsKeyPressed(Input::KeyCode::TWO))
		mCurrentWeapon = mWeapons[1];
	if (input->IsKeyPressed(Input::KeyCode::THREE))
		mCurrentWeapon = mWeapons[2];

	const float moveSpeed = 250.0f;
	if (input->IsKeyDown(Input::KeyCode::W))
	{
		mPosition.y -= moveSpeed *deltaTime ;
	}
	if (input->IsKeyDown(Input::KeyCode::S))
	{
		mPosition.y += moveSpeed * deltaTime;
	}
	if (input->IsKeyDown(Input::KeyCode::D))
	{
		mPosition.x += moveSpeed * deltaTime;
	}
	if (input->IsKeyDown(Input::KeyCode::A))
	{
		mPosition.x -= moveSpeed * deltaTime;
	}

	Math::Vector2 targetPosition = GetTargetPosition();
	mHeading = Math::Normalize(targetPosition - mPosition);

	if (input->IsMouseDown(Input::MouseButton::LBUTTON) && mCurrentWeapon->CanFire() )
	{
		mCurrentWeapon->Fire(mPosition, mHeading);
		EnemyManager::Get().AlertEnemies();
	}
	if (input->IsKeyPressed(Input::KeyCode::R))
	{
		mCurrentWeapon->Reload();
	}

	EnemyManager::Get().CheckPlayerCollision();

}

void Survivor::Render()
{
	if (isAlive)
	{
		float angle = UpdateAngle();
		BatchRender::Get()->AddSprite(mCurrentWeapon->GetSurvivorTextureId(), mPosition, angle);
	}

	Math::Vector2 targetPosition = GetTargetPosition();
	BatchRender::Get()->AddSprite(mCurrentWeapon->GetCrosshairTextureId(), targetPosition);

	std::string str2 = "Health :" + std::to_string(mHealth);
	BatchRender::Get()->AddScreenText(str2.c_str(), 50.0f, 80.0f, 30.0f, Colors::Blue);

	mCurrentWeapon->Render();
}

void Survivor::TakeDamage(int damage)
{
	mHealth -= damage;
	if (mHealth <= 0)
	{
		isAlive = false;
	}
}

Math::Circle Survivor::GetBoundingCircle()
{
	return { mPosition, mCurrentWeapon->GetSurvivorWitdh()};
}

void Survivor::ResetSurvivor()
{
	mHealth = 100;
	isAlive = true;
	mPosition.x = static_cast<float>(GraphicsSystem::Get()->GetBackBufferWidth()) /2.0f;
	mPosition.y = static_cast<float>(GraphicsSystem::Get()->GetBackBufferWidth()) / 2.0f;
}

float Survivor::UpdateAngle()
{
	Math::Vector2 direction = Math::Normalize(mHeading);
	return atan2(direction.y, direction.x);
}
