#pragma once
#include <Angazi/Inc/Angazi.h>

class Weapon;

class Survivor
{
public:
	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();

	void TakeDamage(int damage);

	const Angazi::Math::Vector2 &GetPosition() const { return mPosition; }
	Angazi::Math::Circle GetBoundingCircle();
	bool IsAlive() const { return isAlive; }

	void SetPosition(const Angazi::Math::Vector2 & position) { mPosition = position; }
	void ResetSurvivor();
	float UpdateAngle();

private:
	std::array<Weapon*, 3> mWeapons;
	Weapon* mCurrentWeapon = nullptr;
	Angazi::Math::Vector2 mPosition;
	Angazi::Math::Vector2 mHeading;

	int mHealth = 100;
	bool isAlive = true;

};

