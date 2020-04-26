#pragma once
#include "Character.h"
#include <Angazi/Inc/Angazi.h>

class Enemy : public Character, public Angazi::AI::Agent
{
public:
	Enemy(Angazi::AI::AIWorld& world, uint32_t typeId);
	void Load(std::filesystem::path fileName, bool facingLeft) override;
	void Unload() override;
	void Update(float deltaTime) override;
	void Render() override;

	void ChangeAnimation(Animation2D animation, bool overrideChange = false) override;

	void TakeDamage(int damage) override;
	void Attack() override;

	Angazi::Math::Rect GetBoundingBox() override;

	//
	bool IsAlive() { return isAlive; };
	int GetDamage() { return mDamage; }

	bool mShowDebug = false;

protected:
	Angazi::Math::Vector2 mHeading;
	std::string weaponType;

	float mDetectRange = 200.0f;
	float mStoppingRange = 75.0f;

	float mDeathDelay = 0.0f;
};

