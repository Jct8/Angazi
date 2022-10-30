#include "Ship.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Math;
using namespace Angazi::Input;


void Ship::Load()
{
	mAsteroidManager.Reset();
	mBulletManager.Reset();

	float width = GraphicsSystem::Get()->GetBackBufferHeight() * 0.5f;
	float height = GraphicsSystem::Get()->GetBackBufferHeight() * 0.5f;

	mPosition = { width,height };
	mAlive = true;
}

void Ship::Update(float deltaTime)
{
	if (IsAlive())
	{
		if (mAsteroidManager.Intersect(GetBound()))
		{
			Kill();
		}
		else
		{
			if (brain)
			{
				std::vector<double> vision;
				vision.resize(8);
				constexpr auto step = Constants::TwoPi / 8;
				for (int i = 0; i < 8; i++)
				{
					float radian = i * step;
					auto lineEnd = mPosition + Math::Rotate(mHeading, radian) * maxDistance;
					LineSegment line(mPosition, lineEnd);
					float distance = mAsteroidManager.Intersect(line, maxDistance);

					//if (distance != FLT_MAX)
					{
						vision[i] = distance;
					}
					//else
					//	vision[i] = 0.0;
				}

				auto result = brain->Evaluate(vision);
				if (result[0] > 0.5) // Move forward
					isMoving = true;
				else
					isMoving = false;

				if (result[1] > 0.5) // Move Left
					mHeading = Math::Rotate(mHeading, -0.08f);
				else if (result[2] > 0.5) // Move Right
					mHeading = Math::Rotate(mHeading, 0.08f);

				if (result[3] > 0.5)
					isShooting = true;
				else
					isShooting = false;

				fitness += deltaTime * 10.f;
			}
			else
			{
				auto inputSystem = InputSystem::Get();
				Vector2 mousePos = { (float)inputSystem->GetMouseScreenX(), (float)inputSystem->GetMouseScreenY() };
				mHeading = Normalize(mousePos - mPosition);

				if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
					isMoving = true;
				else
					isMoving = false;

				if (inputSystem->IsMousePressed(MouseButton::LBUTTON))
					isShooting = true;
				else
					isShooting = false;
			}

			if (isMoving)
				mPosition += mHeading * mSpeed * deltaTime;

			mShootDelay -= deltaTime;
			if (mShootDelay <= 0.0f && isShooting)
			{
				mBulletManager.FireBullet(mHeading, mPosition);
				mShootDelay = 0.5f;
			}

			float width = (float)GraphicsSystem::Get()->GetBackBufferWidth();
			float height = (float)GraphicsSystem::Get()->GetBackBufferHeight();

			if (mPosition.x <= 0.0f)
				mPosition.x += width;
			if (mPosition.x > width)
				mPosition.x = 0.0f;
			if (mPosition.y <= 0.0f)
				mPosition.y += height;
			if (mPosition.y > height)
				mPosition.y = 0.0f;
		}
	}

	mBulletManager.Update(deltaTime);
	mAsteroidManager.Update(deltaTime, mBulletManager, *this);
}

void Ship::Render()
{
	//for (int i = 0; i < 8; i++)
	//{
	//	float radian = i * Constants::TwoPi / 8;
	//	auto lineEnd = mPosition + Math::Rotate(mHeading, radian) * maxDistance;
	//	LineSegment line(mPosition, lineEnd);
	//	SimpleDraw::AddScreenLine(line.from, line.to, Colors::Blue);
	//}

	SimpleDraw::AddScreenTriangle(mPosition, mHeading, mRadius, Colors::AliceBlue);
	std::string txt;
	txt += " Current Ship Fitness: " + std::to_string(fitness);
	BatchRenderer::Get()->AddScreenText(txt.c_str(), 900.0f, 10.0f, 20.0f, Colors::White);

	mBulletManager.Render();
	mAsteroidManager.Render();
}


void Ship::Spawn()
{
	mHeading = { 1.0f,0.0f };
	fitness = 0.0f;
	mSpeed = 500.0f;
	mAlive = true;
	Load();
}

void Ship::Kill()
{
	mAlive = false;
}
