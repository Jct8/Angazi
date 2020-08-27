#pragma once
#include "Weapon.h"

class MeleeWeapon : public Weapon
{
public:
	~MeleeWeapon() = default;
	void Load(std::filesystem::path fileName)   override;
	void Unload() override;
	void Render(int mFrame, Angazi::Math::Vector2 screenPos, bool isFacingLeft) override;

	void Attack(int mFrame, Angazi::Math::Vector2 position, bool isFacingLeft, bool isPlayer) override;
	bool IsMelee() const override { return true; }

	int GetFrameCount() override;

private:
	float mHitWidth = 10.0f;
	float mHitHeight = 10.0f;
	int mCurrentAttack = 0;
	std::string mParticleName;

	std::map<int,std::deque<Angazi::Graphics::TextureId>> mAnimationsMap;
	std::map<int, std::vector<Angazi::Math::Vector2>> mAnimationDamageMap;
};

