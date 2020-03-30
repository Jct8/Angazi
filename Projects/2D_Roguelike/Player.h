#ifndef INCLUDED_Player_H
#define INCLUDED_Player_H

#include <AI.h>
#include <XEngine.h>
#include "Character.h"

class TileMap;

enum JumpState
{
	Grounded,
	Jump,
	DoubleJump
};

class Player : public Character, public AI::Agent
{
public:
	Player(AI::AIWorld& world, uint32_t typeId);
	void Load(std::filesystem::path fileName, bool facingLeft) override;
	void Unload() override;
	void Update(float deltaTime) override;
	void Render() override;

	void InitializeAI();
	void AIControl(float deltaTime);
	void PlayerControl(float deltaTime);
	void ChangeAnimation(Animation animation, bool overrideChange = false) override;

	void TakeDamage(int damage) override;
	void Attack() override;

	X::Math::Rect GetBoundingBox() override;
	float GetHealthPercent() { return static_cast<float>(mHealth) / mMaxHealth; }
	X::TextureId GetPrimaryWeaponSprite() { return mWeapon->GetSprite(); }
	X::TextureId GetSecondaryWeaponSprite() { return mSecondaryWeapon->GetSprite(); }
	void Reset();

	void SetDestination(X::Math::Vector2 dest) { mDestination = dest; };
	void SetPosition(const X::Math::Vector2 & pos)override { mPosition = pos; position = { pos.x  , pos.y + mHeight }; };
	void SetCurrentWeapon(int weapon) { mCurrentWeapon = weapon; }

	X::Math::Vector2 GetDestination() { return mDestination; };
	X::Math::Vector2 GetEnemyDestination() { return enemyDestination; };
	Animation GetCurrentAnimation() { return mCurrentAnimation; }
	float GetShootingRange() { return mShootingRange; }
	float GetMeleeRange() { return mMeleeRange; }
	float GetJumpSpeed() { return jumpSpeed; }

	void DebugUI();

	AI::Path path;
	X::Math::Vector2 displacement;
	int currentPath = 0;

private:
	void CalculatePath(X::Math::Vector2 destination);

	int mCurrentWeapon = 0;

	//std::shared_ptr<Weapon> mSecondaryWeapon;
	Weapon* mSecondaryWeapon;
	Weapon* mWeapon;
	JumpState mJumpState = Grounded;

	//AI
	X::Math::Vector2 mDestination;
	X::Math::Vector2 enemyDestination;
	std::unique_ptr<AI::SteeringModule> mSteeringModule;
	std::unique_ptr<AI::StateMachine<Player>> mStateMachine;
	std::unique_ptr<AI::PerceptionModule> mPerceptionModule;

	const float jumpSpeed = 500.0f;
	const float rollSpeed = 500.0f;
	const float attackMoveSpeed = 150.0f;
	float mRollDelay = 0.0f;
	float mViewRange = 250.0f;
	float mDetectRange = 500.0f;
	float mShootingRange = 150.0f;
	float mMeleeRange = 100.0f;
	float mMemorySpan = 5.0f;

	bool mCalculatedFinalDestination = false;
	bool freeMovement = false;
	bool isUsingAi = true;
	bool usingDebug = true;
	bool mChooseDestination = false;
};


#endif // !INCLUDED_Player_H
