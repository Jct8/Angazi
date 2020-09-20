#include "Bird.h"

#include "PipeManager.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Math;

void Bird::Load()
{
	mTextureIds[0] = TextureManager::Get()->Load("FlappyBird/bird1.png");
	mTextureIds[1] = TextureManager::Get()->Load("FlappyBird/bird2.png");
	mTextureIds[2] = TextureManager::Get()->Load("FlappyBird/bird3.png");
	mTextureIds[3] = TextureManager::Get()->Load("FlappyBird/bird2.png");
	mTextureIds[4] = TextureManager::Get()->Load("FlappyBird/bird_die.png");
}

void Bird::Update(float deltaTime, const PipeManager& pm)
{
	if (IsAlive())
	{
		if (pm.Intersect(*this))
		{
			Kill();
		}
		else
		{
			if (brain)
			{
				auto closetPipe = pm.GetClosestPipe(*this);
				auto result = brain->Evaluate({ mPosition.y, closetPipe.GetTopRect().left ,closetPipe.GetTopRect().bottom, closetPipe.GetBottomRect().top });
				if (result[0] > 0.5)
					Flap();
				fitness += deltaTime;
			}

			const float numFrames = static_cast<float>(std::size(mTextureIds) - 1);
			mAnimationFrame += deltaTime * numFrames;
			if (mAnimationFrame >= numFrames)
				mAnimationFrame -= numFrames;

			const float gravity = 5000.0f;
			mVelocity.y += gravity * deltaTime;
			mPosition += mVelocity * deltaTime;
		}
	}
	else
	{
		mAnimationFrame = static_cast<float>(std::size(mTextureIds) - 1);

		const float gravity = 5000.0f;
		mVelocity.y += gravity * deltaTime;
		mVelocity.x = -100.0f;
		mPosition += mVelocity * deltaTime;
	}
}

void Bird::Render()
{
	//SimpleDraw::AddScreenCircle({ mPosition, mRadius }, Colors::Yellow);
	BatchRenderer::Get()->AddSprite(mTextureIds[(int)mAnimationFrame], mPosition);
	if (brain)
	{
		std::string txt;
		txt += "fitness: " + std::to_string(fitness);
		BatchRenderer::Get()->AddScreenText(txt.c_str(), mPosition.x + 40.0f, mPosition.y - 30.0f, 12.0f, Colors::White);
	}
}

void Bird::Flap()
{
	if (IsAlive())
		mVelocity.y = -1100.0f;
}

void Bird::Spawn(const Math::Vector2 & pos)
{
	mPosition = pos;
	mVelocity = Math::Vector2();
	mAlive = true;
}

void Bird::Kill()
{
	mAlive = false;
}
