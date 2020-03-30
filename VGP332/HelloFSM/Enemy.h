#pragma once
#include <Angazi/Inc/Angazi.h>

class Enemy
{
public:
	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();

public:
	void ChangeState(std::string stateName);
	void TakeDamage(int damage);

	void SetPosition(const Angazi::Math::Vector2 & position) { mPosition = position; }
	void SetAlerted(bool alert) { isAlerted = alert; }

	Angazi::Math::Vector2 GetPosition() { return mPosition; }
	Angazi::Math::Circle GetBoundingCircle() { return { mPosition, mTextureId.GetWidth()*0.5f }; }
	bool IsAlive() { return isAlive; };
	int GetDamage() { return mDamage; }
	float GetSpeed() { return mSpeed; }

	bool CanDamage();

protected:
	std::unique_ptr<Angazi::AI::StateMachine<Enemy>> mStateMachine;

	Angazi::Graphics::Texture mTextureId;
	Angazi::Math::Vector2 mPosition;
	Angazi::Math::Vector2 mHeading;
	
	int mDamage = 5;
	int mHealth = 100;
	
	bool isAlive = true;
	bool isAlerted = false;

	float mSpeed = 100.0f;
	float mDamageDelay = 0.0f;
	float mDeathDelay = 0.0f;
};

