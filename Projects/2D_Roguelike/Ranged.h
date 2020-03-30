#pragma once
#include "Weapon.h"

class RangedWeapon :public Weapon
{
public:
	~RangedWeapon() = default;
	void Load(std::filesystem::path fileName)    override;
	void Unload() override;
	void Render(int mFrame, X::Math::Vector2 screenPos, bool isFacingLeft) override;
	void Attack(int mFrame, X::Math::Vector2 screenPos, bool isFacingLeft, bool isPlayer) override;

	bool IsMelee() const override { return false; }

private:
	X::Math::Vector2 mOffset;
	float mAttackDelay = 0;
	int mTotalProjectiles = 1;

};

