#include "TestStates.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;



void RedState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Red);
}

void RedState::Update(float deltatTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
		MainApp().ChangeState("BlueState");
}

void BlueState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Blue);
}

void BlueState::Update(float deltatTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
		MainApp().ChangeState("RedState");
}
