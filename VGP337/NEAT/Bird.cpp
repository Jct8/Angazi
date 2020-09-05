#include "Bird.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;


void Bird::Update(float deltaTime)
{
	const float gravity = 3000.0f;
	mVelocity.y += gravity * deltaTime;
	mPosition += mVelocity * deltaTime;
}

void Bird::Render()
{
	SimpleDraw::AddScreenCircle({ mPosition, mRadius }, Colors::Yellow);
}

void Bird::Flap()
{
	mVelocity.y = -500.0f;
}