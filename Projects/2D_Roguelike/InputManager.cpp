#include "InputManager.h"

using namespace Angazi::Input;

namespace
{
	std::unique_ptr<InputManager> sInstance = nullptr;
	const int defaultSize = 50;
}

void InputManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "InputManager already initialized");
	sInstance = std::make_unique<InputManager>();
}

void InputManager::StaticTerminate()
{
	sInstance.reset();
}

InputManager & InputManager::Get()
{
	ASSERT(sInstance != nullptr, "No InputManager created!");
	return *sInstance;
}

bool InputManager::AttackInput()
{
	return InputSystem::Get()->IsMousePressed(MouseButton::LBUTTON) || InputSystem::Get()->IsYPressed(0);
}

bool InputManager::SecondaryAttackInput()
{
	return InputSystem::Get()->IsMousePressed(MouseButton::RBUTTON) || InputSystem::Get()->IsXPressed(0);
}

bool InputManager::RollInput()
{
	return InputSystem::Get()->IsKeyPressed(KeyCode::LSHIFT) || InputSystem::Get()->IsAPressed(0);
}

bool InputManager::HorizontalLeftInput()
{
	return InputSystem::Get()->IsKeyDown(KeyCode::D);
}

bool InputManager::HorizontalRightInput()
{
	return InputSystem::Get()->IsKeyDown(KeyCode::A) ;
}

bool InputManager::JumpInput()
{
	return InputSystem::Get()->IsKeyPressed(KeyCode::SPACE) || InputSystem::Get()->IsBPressed(0);
}
