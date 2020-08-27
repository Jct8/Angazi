#include "ImGui/Inc/imgui.h"
#include "Camera2D.h"
#include "TileMap.h"
#include "EnemyManager.h"
#include "ProjectileManager.h"
#include "WeaponManager.h"
#include "Player.h"
#include "Game.h"
#include "InputManager.h"
#include "MoveState.h"
#include "FightState.h"
#include "IdleState.h"
#include "SightSensor.h"

extern Game gameState;
using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	const int maxsize = 100;
	Animation2D ConvertToEnum(std::string name)
	{
		if (name == "Idle") return Idle;
		if (name == "Attacking") return Attacking;
		if (name == "Running")  return Running;
		if (name == "Jumping")  return Jumping;
		if (name == "Death")    return Death;
		if (name == "Climbing") return Climbing;
		if (name == "Roll") return Roll;
		if (name == "Fall") return Fall;
		return Idle;
	}
}

Player::Player(Angazi::AI::AIWorld& world, uint32_t typeId)
	:Agent(world, typeId)
{

}

void Player::Load(std::filesystem::path fileName, bool facingLeft)
{
	char animString[maxsize];
	char name[maxsize];
	int total = 0;
	int subTotal = 0;
	float duration;

	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");
	fscanf_s(file, "Total:%d\n", &total);
	fscanf_s(file, "Width:%f\n", &mWidth);
	fscanf_s(file, "Height:%f\n", &mHeight);
	fscanf_s(file, "Speed:%f\n", &mSpeed);

	for (int i = 0; i < total; i++)
	{
		fscanf_s(file, "%s %d %f\n", animString, maxsize, &subTotal, &duration);
		std::string animS(animString);
		Animation2D anim = ConvertToEnum(animS);
		mAnimationsDuration[anim] = duration;

		for (int j = 0; j < subTotal; j++)
		{
			fscanf_s(file, "%s\n", name, maxsize);
			mAnimations[anim].emplace_back();
			mAnimations[anim].back()= TextureManager::Get()->Load("../../Assets/Images/Rougelike/" + std::string(name));
		}
	}
	fclose(file);

	mDuration = 0.5f;
	mFrame = 0;
	mFrameCount = static_cast<int>(mAnimations[Idle].size());
	mFrameTimer = mAnimationsDuration[Idle] / mFrameCount;

	mCurrentAnimation = Idle;

	mSecondaryWeapon = WeaponManager::Get().GetWeapon("Weapon2").get();
	mWeapon = WeaponManager::Get().GetWeapon("Weapon1").get();
}

void Player::Unload()
{
	for (auto& animationType : mAnimations)
		for (auto& animation : animationType.second)
			animation = 0;
}

void Player::Update(float deltaTime)
{
	auto input = Input::InputSystem::Get();
	if (input->IsMouseDown(Input::MouseButton::LBUTTON))
	{
		if (mChooseDestination)
		{
			mChooseDestination = false;
			auto mDest = Camera2D::Get().ConvertToWorldPosition({ static_cast<float>(input->GetMouseScreenX()) ,static_cast<float>(input->GetMouseScreenY()) });
			float column = mDest.x / 32.0f;
			float row = mDest.y / 32.0f;
			mDestination = { column,row };
			CalculatePath(mDestination);
			mCalculatedFinalDestination = true;
			mStateMachine->ChangeState("MoveState");
		}
	}

	displacement = 0.0f;
	if (freeMovement)
	{
		if (input->IsKeyDown(Input::KeyCode::W))
		{
			displacement.y = -500 * deltaTime;
		}
		if (input->IsKeyDown(Input::KeyCode::S))
		{
			displacement.y = 500 * deltaTime;
		}
		if (input->IsKeyDown(Input::KeyCode::D))
		{
			displacement.x = 500 * deltaTime;
		}
		if (input->IsKeyDown(Input::KeyCode::A))
		{
			displacement.x = -500 * deltaTime;
		}
		mPosition += displacement;
		return;
	}

	if (isUsingAi)
		AIControl(deltaTime);
	else
		PlayerControl(deltaTime);
}

void Player::Render()
{
	Math::Vector2 screenPos = Camera2D::Get().ConvertToScreenPosition(mPosition);

	if (mCurrentAnimation == Attacking && mCurrentWeapon == 1)
		mWeapon->Render(mFrame, screenPos, isFacingLeft);
	else if (mCurrentAnimation == Attacking && mCurrentWeapon == 2)
		mSecondaryWeapon->Render(mFrame, screenPos, isFacingLeft);
	else if (isFacingLeft)
		BatchRender::Get()->AddSprite(mAnimations[mCurrentAnimation][mFrame], screenPos);
	else
		BatchRender::Get()->AddSprite(mAnimations[mCurrentAnimation][mFrame], screenPos, 0.0f, Pivot::Center, Flip::Horizontal);

	if (mFrame == mFrameCount - 1 && mJumpState == Grounded && isAlive)
	{
		if (!isUsingAi || (isUsingAi && mCurrentWeapon == 1))
			ChangeAnimation(Idle);
		mCurrentWeapon = 0;
		mWeapon->SetDealtDamage(false);
		mSecondaryWeapon->SetDealtDamage(false);
	}

	if (usingDebug)
	{
		float offset = 32.0f * 0.5f;
		//Hitbox
		//X::Math::Rect rect = Camera::Get().ConvertToScreenPosition(GetBoundingBox());
		//X::DrawScreenRect(rect, X::Colors::Green);

		SimpleDraw::AddScreenCircle({ screenPos.x, screenPos.y }, mViewRange, Colors::AliceBlue);
		//X::DrawScreenCircle({ screenPos.x, screenPos.y }, mDetectRange, X::Colors::LightBlue);

		Math::Vector2 screenPos2 = Camera2D::Get().ConvertToScreenPosition(Math::Vector2{ position.x ,position.y });
		SimpleDraw::AddScreenCircle({ screenPos2.x, screenPos2.y - offset }, 10.0f, Colors::AliceBlue);

		Math::Vector2 screenPosDestination = Camera2D::Get().ConvertToScreenPosition(Math::Vector2{ destination.x, destination.y });
		SimpleDraw::AddScreenCircle({ screenPosDestination.x, screenPosDestination.y - offset }, 10.0f, Colors::Green);

		Math::Vector2 screenPosEnemy = Camera2D::Get().ConvertToScreenPosition(Math::Vector2{ enemyDestination.x ,enemyDestination.y });
		SimpleDraw::AddScreenCircle({ screenPosEnemy.x, screenPosEnemy.y - offset }, 10.0f, Colors::Red);

		if (!path.empty())
		{
			for (size_t i = 0; i < path.size() - 1; i++)
			{
				Math::Vector2 screenPos = Camera2D::Get().ConvertToScreenPosition(Math::Vector2{ static_cast<float>(path[i].x)*32.0f , static_cast<float>(path[i].y*32.0f) });
				Math::Vector2 screenPos2 = Camera2D::Get().ConvertToScreenPosition(Math::Vector2{ static_cast<float>(path[i + 1].x)*32.0f , static_cast<float>(path[i + 1].y)*32.0f });
				SimpleDraw::AddScreenLine(screenPos.x + offset, screenPos.y + offset, screenPos2.x + offset, screenPos2.y + offset, Colors::AliceBlue);
			}
		}

	}
}

void Player::InitializeAI()
{
	// SteeringModule
	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
	mSteeringModule->AddBehavior<AI::ArriveBehavior>("Arrive")->SetActive(true);
	maxSpeed = mSpeed;
	mSteeringModule->GetBehavior<AI::ArriveBehavior>("Arrive")->decelerationTweaker = 0.05f;
	mSteeringModule->GetBehavior<AI::ArriveBehavior>("Arrive")->deceleration = 1;

	// State Machine
	mStateMachine = std::make_unique<AI::StateMachine<Player>>(*this);
	mStateMachine->AddState<MoveState>("MoveState");
	mStateMachine->AddState<IdleState>("IdleState");
	mStateMachine->AddState<FightState>("FightState");
	mStateMachine->ChangeState("MoveState");

	// Perception Module
	AI::ImportanceCalculator importanceCalculator =
		[](const Agent& agent, AI::MemoryRecord& record)
	{
		record.importance = Math::DistanceSqr(agent.position, std::get<Math::Vector2>(record.properties["lastSeenPosition"]));
	};
	mPerceptionModule = std::make_unique<AI::PerceptionModule>(*this, importanceCalculator);
	mPerceptionModule->AddSensor<SightSensor>("SightSensor");
	mPerceptionModule->GetSensor<SightSensor>("SightSensor")->viewRange = mViewRange;
	mPerceptionModule->GetSensor<SightSensor>("SightSensor")->neighborhoodRadius = mDetectRange;
	mPerceptionModule->SetMemorySpan(mMemorySpan);
}

void Player::AIControl(float deltaTime)
{
	if (!isAlive)
	{
		gameState = GameLose;
		return;
	}
	auto input = Input::InputSystem::Get();

	if (input->IsKeyPressed(Input::KeyCode::ONE) && mCurrentAnimation != Attacking)
		mWeapon = WeaponManager::Get().GetWeapon("Weapon1").get();
	if (input->IsKeyPressed(Input::KeyCode ::TWO) && mCurrentAnimation != Attacking)
		mSecondaryWeapon = WeaponManager::Get().GetWeapon("Weapon2").get();
	if (input->IsKeyPressed(Input::KeyCode ::THREE) && mCurrentAnimation != Attacking)
		mSecondaryWeapon = WeaponManager::Get().GetWeapon("Weapon3").get();
	if (input->IsKeyPressed(Input::KeyCode::FOUR) && mCurrentAnimation != Attacking)
		mWeapon = WeaponManager::Get().GetWeapon("Weapon4").get();

	displacement = 0.0f;

	mPerceptionModule->Update(deltaTime);
	AI::MemoryRecords memoryRecords = mPerceptionModule->GetMemoryRecords();
	if (!memoryRecords.empty())
	{
		auto newDest = std::get<Math::Vector2>(memoryRecords.back().properties["lastSeenPosition"]);
		if ((newDest.x != enemyDestination.x || newDest.y != enemyDestination.y) && mCurrentAnimation != Attacking)
		{
			enemyDestination = newDest;
			float xDest = enemyDestination.x / 32.0f;
			float yDest = (enemyDestination.y) / 32.0f;
			CalculatePath({ xDest,yDest });
			mCalculatedFinalDestination = false;
			mStateMachine->ChangeState("MoveState");
		}
		enemyDestination = newDest;
	}
	else if (!mCalculatedFinalDestination)
	{
		CalculatePath(mDestination);
		mCalculatedFinalDestination = true;
		mStateMachine->ChangeState("MoveState");
	}
	if (!mCalculatedFinalDestination
		&& Math::Distance(position, enemyDestination) < mShootingRange && mCurrentAnimation != Attacking && mJumpState == Grounded
		&& static_cast<int>(position.y / 32) == static_cast<int>(enemyDestination.y / 32))
	{
		mStateMachine->ChangeState("FightState");
	}
	if (!mCalculatedFinalDestination
		&&  mCurrentAnimation == Attacking
		&& static_cast<int>(position.y / 32) != static_cast<int>(enemyDestination.y / 32))
	{
		float xDest = enemyDestination.x / 32.0f;
		float yDest = (enemyDestination.y) / 32.0f;
		CalculatePath({ xDest , yDest });
		mStateMachine->ChangeState("MoveState");
	}

	mStateMachine->Update(deltaTime);

	////Move forward when attacking
	//if (mCurrentAnimation == Attacking)
	//{
	//	if (isFacingLeft && mCurrentWeapon == 1)
	//		displacement.x -= attackMoveSpeed * deltaTime;
	//	else if (mCurrentWeapon == 1)
	//		displacement.x += attackMoveSpeed * deltaTime;
	//}

#pragma region Collisions
	Math::Rect currentBox
	{
		mPosition.x - mWidth,
		mPosition.y - mHeight,
		mPosition.x + mWidth,
		mPosition.y + mHeight
	};
	//if (displacement.x > 0.0f)
	//{
	//	X::Math::LineSegment rightEdge{
	//		currentBox.max.x + displacement.x,
	//		currentBox.min.y,
	//		currentBox.max.x + displacement.x,
	//		currentBox.max.y,
	//	};
	//	if (TileMap::Get().CheckCollision(rightEdge))
	//	{
	//		displacement.x = 0.0f;
	//		/*displacement.y = -moveSpeed * deltaTime;
	//		ChangeAnimation(Climbing);*/
	//	}
	//}
	//if (displacement.x < 0.0f)
	//{
	//	X::Math::LineSegment leftEdge{
	//		currentBox.min.x + displacement.x,
	//		currentBox.min.y,
	//		currentBox.min.x + displacement.x,
	//		currentBox.max.y,
	//	};

	//	if (TileMap::Get().CheckCollision(leftEdge))
	//	{
	//		displacement.x = 0.0f;
	//		/*displacement.y = -moveSpeed * deltaTime;
	//		ChangeAnimation(Climbing);*/
	//	}

	//}

	if (displacement.y > 0.0f)
	{
		Math::LineSegment BottomEdge{
			currentBox.left,
			currentBox.bottom + displacement.y,
			currentBox.right,
			currentBox.bottom + displacement.y,
		};
		if (TileMap::Get().CheckCollision(BottomEdge))
		{
			displacement.y = 0.0f;
			mJumpState = Grounded;
		}
	}
	//if (displacement.y < 0.0f)
	//{
	//	X::Math::LineSegment TopEdge{
	//		currentBox.min.x,
	//		currentBox.min.y + displacement.y,
	//		currentBox.max.x,
	//		currentBox.min.y + displacement.y,
	//	};

	//	if (TileMap::Get().CheckCollision(TopEdge))
	//	{
	//		displacement.y = 0.0f;
	//		displacement.x = 0.0f;
	//	}
	//}
#pragma endregion

	if (displacement.y < 0.0f && mCurrentAnimation != Attacking)
	{
		mJumpState = Jump;
		ChangeAnimation(Jumping);
	}
	else if (displacement.y > 0.0f && mCurrentAnimation != Attacking)
	{
		mJumpState = Jump;
		ChangeAnimation(Fall);
	}
	else if (displacement.x != 0.0f)
		ChangeAnimation(Running);

	if (displacement.x > 0.0001f)
		isFacingLeft = false;
	else if (displacement.x < -0.9999f)
		isFacingLeft = true;

	mFrameTimer -= deltaTime;
	if (mFrameTimer <= 0.0f)
	{
		mFrameTimer = mAnimationsDuration[mCurrentAnimation] / mFrameCount;
		mFrame = (mFrame + 1) % mFrameCount;
	}

	if (currentPath == path.size() && mCalculatedFinalDestination)
		mStateMachine->ChangeState("IdleState");

	mPosition += displacement;
	position += displacement;
}

void Player::PlayerControl(float deltaTime)
{
	if (!isAlive)
	{
		gameState = GameLose;
		return;
	}

	auto input = Input::InputSystem::Get();
	if (input->IsKeyPressed(Input::KeyCode::ONE) && mCurrentAnimation != Attacking)
		mWeapon = WeaponManager::Get().GetWeapon("Weapon1").get();
	if (input->IsKeyPressed(Input::KeyCode::TWO) && mCurrentAnimation != Attacking)
		mSecondaryWeapon = WeaponManager::Get().GetWeapon("Weapon2").get();
	if (input->IsKeyPressed(Input::KeyCode::THREE) && mCurrentAnimation != Attacking)
		mSecondaryWeapon = WeaponManager::Get().GetWeapon("Weapon3").get();
	if (input->IsKeyPressed(Input::KeyCode::FOUR) && mCurrentAnimation != Attacking)
		mWeapon = WeaponManager::Get().GetWeapon("Weapon4").get();

	Math::Vector2 displacement;

#pragma region Jump
	//Jump
	if (InputManager::Get().JumpInput() && (mJumpState == Grounded || mJumpState == Jump)
		&& mCurrentAnimation != Roll)
	{
		if (mJumpState == Grounded)
			mJumpState = Jump;
		else
		{
			ChangeAnimation(Idle);
			mJumpState = DoubleJump;
		}
		mJumpDelay = MainApp().GetTime() + 0.2f;
	}
	if (mJumpDelay > MainApp().GetTime())
		displacement.y = -jumpSpeed * deltaTime;
	else
		displacement.y = mSpeed * deltaTime;

	if (mJumpState != Grounded && mCurrentAnimation != Climbing
		&& mCurrentAnimation != Attacking && mCurrentAnimation != Roll)
	{
		ChangeAnimation(Jumping);
	}
	if (mJumpState != Grounded && mCurrentAnimation == Jumping && displacement.y > 0.0f)
	{
		ChangeAnimation(Fall);
	}
#pragma endregion

#pragma region Left Or Right Movement
	//Left or Right
	if (InputManager::Get().HorizontalLeftInput() && mCurrentAnimation != Attacking && mCurrentAnimation != Roll)
	{
		displacement.x = mSpeed * deltaTime;
		//if (isGrounded && !isJumping)
		if (mJumpState == Grounded && mCurrentAnimation != Jumping && mCurrentAnimation != Climbing)
			ChangeAnimation(Running);

		isFacingLeft = false;
	}
	if (InputManager::Get().HorizontalRightInput() && mCurrentAnimation != Attacking && mCurrentAnimation != Roll)
	{
		displacement.x = -mSpeed * deltaTime;
		//if (isGrounded && !isJumping)
		if (mJumpState == Grounded && mCurrentAnimation != Jumping && mCurrentAnimation != Climbing)
			ChangeAnimation(Running);
		isFacingLeft = true;
	}
#pragma endregion

#pragma region Attacking
	//Attacking
	if (InputManager::Get().AttackInput() && mCurrentWeapon == 0
		&& mCurrentAnimation != Attacking && mCurrentAnimation != Roll)
	{
		mCurrentWeapon = 1;
		ChangeAnimation(Attacking);
	}
	if (InputManager::Get().SecondaryAttackInput() && mCurrentWeapon == 0
		&& mCurrentAnimation != Attacking && mCurrentAnimation != Roll)
	{
		mCurrentWeapon = 2;
		ChangeAnimation(Attacking);
	}
#pragma endregion

#pragma region Roll
	//Roll
	if (InputManager::Get().RollInput() && mCurrentAnimation != Roll)
	{
		ChangeAnimation(Roll);
		mRollDelay = MainApp().GetTime() + 0.4f;
	}
	if (mCurrentAnimation == Roll)
	{
		if (isFacingLeft)
			displacement.x = -rollSpeed * deltaTime;
		else
			displacement.x = rollSpeed * deltaTime;
		if (mRollDelay < MainApp().GetTime())
		{
			ChangeAnimation(Idle);
		}
	}
#pragma endregion

	mFrameTimer -= deltaTime;
	if (mFrameTimer <= 0.0f)
	{
		mFrameTimer = mAnimationsDuration[mCurrentAnimation] / mFrameCount;
		mFrame = (mFrame + 1) % mFrameCount;
	}

	//Move forward when attacking
	if (mCurrentAnimation == Attacking)
	{
		if (isFacingLeft && mCurrentWeapon == 1)
			displacement.x -= attackMoveSpeed * deltaTime;
		else if (mCurrentWeapon == 1)
			displacement.x += attackMoveSpeed * deltaTime;
		Attack();
	}

#pragma region Collisions
	Math::Rect currentBox
	{
		mPosition.x - mWidth,
		mPosition.y - mHeight,
		mPosition.x + mWidth,
		mPosition.y + mHeight
	};
	if (displacement.x > 0.0f)
	{
		Math::LineSegment rightEdge{
			currentBox.right + displacement.x,
			currentBox.top,
			currentBox.right + displacement.x,
			currentBox.bottom,
		};
		if (TileMap::Get().CheckCollision(rightEdge))
		{
			displacement.x = 0.0f;
			/*displacement.y = -moveSpeed * deltaTime;
			ChangeAnimation(Climbing);*/
		}
	}
	if (displacement.x < 0.0f)
	{
		Math::LineSegment leftEdge{
			currentBox.left + displacement.x,
			currentBox.top,
			currentBox.left + displacement.x,
			currentBox.bottom,
		};

		if (TileMap::Get().CheckCollision(leftEdge))
		{
			displacement.x = 0.0f;
			/*displacement.y = -moveSpeed * deltaTime;
			ChangeAnimation(Climbing);*/
		}

	}

	if (displacement.y > 0.0f)
	{
		Math::LineSegment BottomEdge{
			currentBox.left,
			currentBox.bottom + displacement.y,
			currentBox.right,
			currentBox.bottom + displacement.y,
		};
		if (TileMap::Get().CheckCollision(BottomEdge))
		{
			displacement.y = 0.0f;
			mJumpState = Grounded;
		}
	}
	if (displacement.y < 0.0f)
	{
		Math::LineSegment TopEdge{
			currentBox.left,
			currentBox.top + displacement.y,
			currentBox.right,
			currentBox.top + displacement.y,
		};

		if (TileMap::Get().CheckCollision(TopEdge))
		{
			displacement.y = 0.0f;
			displacement.x = 0.0f;
		}
	}
#pragma endregion

	mPosition += displacement;
	position += displacement;
}

void Player::ChangeAnimation(Animation2D animation, bool overrideChange)
{
	if (animation == mCurrentAnimation && overrideChange == false)
		return;
	mCurrentAnimation = animation;
	if (mCurrentAnimation == Attacking && mCurrentWeapon == 1)
		mFrameCount = mWeapon->GetFrameCount();
	else if (mCurrentAnimation == Attacking && mCurrentWeapon == 2)
		mFrameCount = mSecondaryWeapon->GetFrameCount();
	else
		mFrameCount = static_cast<int>(mAnimations[animation].size());

	mFrame = 0;
	mFrameTimer = mAnimationsDuration[mCurrentAnimation] / mFrameCount;
}

void Player::TakeDamage(int damage)
{
	mHealth -= damage;
	if (mHealth <= 0 && isAlive)
	{
		mHealth = 0;
		isAlive = false;
		ChangeAnimation(Death);
	}
}

void Player::Attack()
{
	if (mCurrentWeapon == 1)
		mWeapon->Attack(mFrame, mPosition, isFacingLeft, true);
	else if (mCurrentWeapon == 2)
		mSecondaryWeapon->Attack(mFrame, mPosition, isFacingLeft, true);
}

Math::Rect Player::GetBoundingBox()
{
	if (mCurrentAnimation == Roll)
		return { 0.0f ,0.0f ,0.0f ,0.0f };

	return
	{
		mPosition.x - mWidth,
		mPosition.y - mHeight,
		mPosition.x + mWidth,
		mPosition.y + mHeight
	};
}

void Player::Reset()
{
	isFacingLeft = true;
	isGrounded = false;
	isJumping = false;
	isAlive = true;

	mCurrentWeapon = 0;
	mDuration = 0.5f;
	mFrame = 0;
	mFrameCount = static_cast<int>(mAnimations[Idle].size());
	mFrameTimer = mAnimationsDuration[Idle] / mFrameCount;

	mCurrentAnimation = Idle;

	mHealth = mMaxHealth;
	isAlive = true;

	mWeapon = WeaponManager::Get().GetWeapon("Weapon1").get();
	mSecondaryWeapon = WeaponManager::Get().GetWeapon("Weapon2").get();
}

void Player::CalculatePath(Math::Vector2 destination)
{
	float x = position.x / 32.0f;
	float y = (position.y) / 32.0f;
	path = TileMap::Get().GetPath({ x,y }, destination);
	currentPath = 1;
}

void Player::DebugUI()
{
	ImGui::Begin("Settings Editor", nullptr);

	if (ImGui::CollapsingHeader("Player:",ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::SliderFloat("Player Speed", &maxSpeed, 0.0f, 1000.0f);
		if (ImGui::SliderFloat("View Range", &mViewRange, 0.0f, 700.0f))
		{
			mPerceptionModule->GetSensor<SightSensor>("SightSensor")->viewRange = mViewRange;
		}
		if (ImGui::SliderFloat("Detect Range", &mDetectRange, 0.0f, 700.0f))
		{
			mPerceptionModule->GetSensor<SightSensor>("SightSensor")->neighborhoodRadius = mDetectRange;
		}
		if (ImGui::SliderFloat("Memory Span", &mMemorySpan, 0.0f, 180.0f))
		{
			mPerceptionModule->SetMemorySpan(mMemorySpan);
		}
		ImGui::Checkbox("Use Free Movement", &freeMovement);
		ImGui::Checkbox("Use AI", &isUsingAi);
		ImGui::Checkbox("Debug UI", &usingDebug);
		if (ImGui::Button("Place Destination Position"))
		{
			mChooseDestination = true;
		}
	}

	ImGui::End();
}