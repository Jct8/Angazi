#pragma once
#include <Angazi/Inc/Angazi.h>

class InputManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static InputManager& Get();

	bool AttackInput();
	bool SecondaryAttackInput();
	bool RollInput();
	bool HorizontalLeftInput();
	bool HorizontalRightInput();
	bool JumpInput();

private:


};