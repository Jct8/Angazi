#pragma once
#include "AnimationDefintion.h"
#include "Melee.h"
#include "Ranged.h"
#include "Particle.h"

class Character
{
public:
	virtual void Load(std::filesystem::path fileName, bool facingLeft) = 0;
	virtual void Unload() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	virtual void ChangeAnimation(Animation animation, bool overrideChange = false) = 0;

	virtual void TakeDamage(int damage) = 0;
	virtual void Attack() = 0;

	//Setters
	virtual void SetPosition(const X::Math::Vector2 & position) { mPosition = position; }
	void SetFacingLeft(bool facing) { isFacingLeft = facing; }


	//Getters
	const X::Math::Vector2& GetPosition() const { return mPosition; }
	virtual X::Math::Rect GetBoundingBox() = 0;
	bool IsFacingLeft() { return isFacingLeft; }
	float GetHeight() { return mHeight; };
	float GetWidth() { return mWidth; };

protected:
	int mDamage = 20;
	int mHealth = 200;
	int mMaxHealth =200;
	float mSpeed = 100.0f;
	float mWidth = 10.0f;
	float mHeight = 10.0f;

	bool isFacingLeft = true;
	bool isGrounded = false;
	bool isJumping = false;
	bool isAlive = true;

	int mFrame = 0;
	int mFrameCount = 0;
	float mDuration = 0.0f;
	float mFrameTimer = 0.0f;

	float mJumpDelay = 0.0f;
	float mAttackDelay = 0.0f;

	Animation mCurrentAnimation;
	std::map<Animation, std::vector<X::TextureId>> mAnimations;
	std::map<Animation, float> mAnimationsDuration;

	X::Math::Vector2 mPosition;
	std::unique_ptr<Weapon> mWeapon;

};

