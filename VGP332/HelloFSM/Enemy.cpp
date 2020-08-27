#include "Enemy.h"
#include "Survivor.h"
#include "IdleState.h"
#include "FollowState.h"
#include "RunState.h"
#include "EnemyManager.h"

extern Survivor survivor;

using namespace Angazi;
using namespace Angazi::Graphics;

void Enemy::Load()
{
	mStateMachine = std::make_unique<AI::StateMachine<Enemy>>(*this);
	mStateMachine->AddState<IdleState>("Idle");
	mStateMachine->AddState<FollowState>("Follow");
	mStateMachine->AddState<RunState>("Run");
	mTextureId = TextureManager::Get()->Load("../../Assets/Images/XEngine/zombie_idle.png");
}

void Enemy::Unload()
{
	mTextureId = 0;
}

void Enemy::Update(float deltaTime)
{
	if (!survivor.IsAlive())
		return;
	if (isAlive)
	{
		mStateMachine->Update(deltaTime);

		if (Math::Distance(survivor.GetPosition(),mPosition) < 500.0f)
		{
			mStateMachine->ChangeState("Run");
		}
		else if (isAlerted)
		{
			mStateMachine->ChangeState("Follow");
		}
		else
		{
			mStateMachine->ChangeState("Idle");
		}
	}
	else
	{
		if (mDeathDelay < MainApp().GetTime())
		{
			Unload();
		}
	}
}

void Enemy::Render()
{
	float angle = atan2(mHeading.y, mHeading.x);
	Graphics::BatchRender::Get()->AddSprite(mTextureId, mPosition, angle);
}

void Enemy::ChangeState(std::string stateName)
{
	mStateMachine->ChangeState(stateName);
}

void Enemy::TakeDamage(int damage)
{
	mHealth -= damage;
	if (mHealth <= 0 && isAlive)
	{
		mTextureId = 0;
		mTextureId = TextureManager::Get()->Load("../../Assets/Images/XEngine/zombie_dead.png");
		isAlive = false;
		EnemyManager::Get().IncreaseDeathCount();
		mDeathDelay = MainApp().GetTime() + 4.0f;
	}
}

bool Enemy::CanDamage()
{
	float time = MainApp().GetTime();
	if (mDamageDelay < time)
	{
		mDamageDelay = time + 1.0f;
		return true;
	}
	return false;
}
