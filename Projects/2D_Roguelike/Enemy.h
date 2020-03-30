#pragma once
#include <AI.h>
#include <XEngine.h>
#include "Character.h"

class Enemy : public Character, public AI::Agent
{
public:
	Enemy(AI::AIWorld& world, uint32_t typeId);
	void Load(std::filesystem::path fileName, bool facingLeft) override;
	void Unload() override;
	void Update(float deltaTime) override;
	void Render() override;

	void ChangeAnimation(Animation animation, bool overrideChange = false) override;

	void TakeDamage(int damage) override;
	void Attack() override;

	X::Math::Rect GetBoundingBox() override;

	//
	bool IsAlive() { return isAlive; };
	int GetDamage() { return mDamage; }

	bool mShowDebug = false;

protected:
	X::Math::Vector2 mHeading;
	std::string weaponType;

	float mDetectRange = 200.0f;
	float mStoppingRange = 75.0f;

	float mDeathDelay = 0.0f;
};

