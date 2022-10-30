#pragma once

#include "Weapon.h"

class Handgun : public Weapon
{
public:
	~Handgun() = default;

	void Load() override;
	void Unload() override;
	void Render() override;

	void Fire(const Angazi::Math::Vector2 &position, const Angazi::Math::Vector2 &direction)  override;
	bool CanFire()  override;
	void Reload() override;
};
