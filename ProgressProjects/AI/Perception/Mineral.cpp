#include "Mineral.h"

#include "TypeIds.h"

using namespace Angazi;
using namespace Angazi::Graphics;

Mineral::Mineral(AI::AIWorld& world)
	: AI::Entity(world, TypeIds::Mineral)
{
}

void Mineral::Initialize()
{
	mTextureId = TextureManager::Get()->Load("XEngine/minerals.png", false, true);
	mEyeTextureId = TextureManager::Get()->Load("XEngine/eye.png", false, true);
	position = Math::RandomVector2({ 100.0f, 100.0f }, { 1180.0f, 710.0f });
}

void Mineral::Render()
{
	if (!collected)
	{
		BatchRenderer::Get()->AddSprite(mTextureId, position);
		if (seen)
			BatchRenderer::Get()->AddSprite(mEyeTextureId, position + Math::Vector2{ 20.0f, -20.0f });
	}
}