#pragma once

#include "Weapon.h"

class Rifle : public Weapon
{
public:
	~Rifle() = default;

	void Load() override;
	void Unload() override;
	void Render() override;

	void Fire(const Angazi::Math::Vector2 &position, const Angazi::Math::Vector2 &direction)  override;
	bool CanFire() override;
	void Reload() override;
};
