#include "UIManager.h"
#include "Player.h"

extern Player player;

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	std::unique_ptr<UIManager> sInstance = nullptr;
}

void UIManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "UIManager already initialized");
	sInstance = std::make_unique<UIManager>();
}

void UIManager::StaticTerminate()
{
	sInstance->Unload();
	sInstance.reset();
}

UIManager & UIManager::Get()
{
	ASSERT(sInstance != nullptr, "No UIManager created!");
	return *sInstance;
}

void UIManager::Load()
{
	barEmpty = TextureManager::Get()->Load("../../Assets/Images/Rougelike/bar_empty.png");
	barRed = TextureManager::Get()->Load("../../Assets/Images/Rougelike/bar_red.png");
}

void UIManager::Unload()
{
	barEmpty = 0;
	barRed = 0;
}

void UIManager::Update(float deltaTime)
{

}

void UIManager::Render()
{
	playerHealthPercent = player.GetHealthPercent();
	Math::Rect rect
	{
		0.0f,0.0f,
		playerHealthPercent * TextureManager::Get()->GetTexture(barRed)->GetWidth(),
		1.0f * TextureManager::Get()->GetTexture(barRed)->GetHeight()
	};

	BatchRenderer::Get()->AddSprite(barEmpty, { 25.0f,680.0f }, 0.0f, Pivot::TopLeft);
	BatchRenderer::Get()->AddSprite(barRed, rect, { 25.0f,680.0f }, 0.0f, Pivot::TopLeft, Flip::None);

	SimpleDraw::AddScreenRect(30.0f, 610.0f, 90.0f, 670.0f, Colors::AliceBlue);
	SimpleDraw::AddScreenRect(95.0f, 610.0f, 155.0f, 670.0f, Colors::AliceBlue);
	BatchRenderer::Get()->AddSprite(player.GetPrimaryWeaponSprite(), { 35.0f,620.0f }, 0.0f, Pivot::TopLeft);
	BatchRenderer::Get()->AddSprite(player.GetSecondaryWeaponSprite(), { 100.0f,620.0f }, 0.0f, Pivot::TopLeft);
}
