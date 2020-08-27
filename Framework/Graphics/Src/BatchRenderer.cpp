#include "Precompiled.h"
#include "BatchRenderer.h"

#include "TextureManager.h"

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	struct TextCommand
	{
		TextCommand(std::wstring text, float size, float x, float y, Color color)
			:text(std::move(text)), size(size), x(x), y(y), color(color)
		{}

		std::wstring text;
		float size, x, y;
		Color color;
	};

	struct SpriteCommand
	{
		SpriteCommand(TextureId textureId, const Math::Vector2& posiiton, float rotation)
			: textureId(textureId) , sourceRect(Math::Rect()), position(posiiton), rotation(rotation)
		{}

		SpriteCommand(TextureId textureId, const Math::Vector2& posiiton, float rotation, Pivot pivot, Flip flip)
			: textureId(textureId), sourceRect(Math::Rect()), position(posiiton)
			, rotation(rotation), pivot(pivot)	, flip(flip)
		{}

		SpriteCommand(TextureId textureId, const Math::Rect& sourceRect, const Math::Vector2& posiiton, float rotation)
			: textureId(textureId), sourceRect(sourceRect), position(posiiton), rotation(rotation)
		{}

		SpriteCommand(TextureId textureId, const Math::Rect& sourceRect, const Math::Vector2& posiiton, float rotation, Pivot pivot, Flip flip)
			: textureId(textureId), sourceRect(sourceRect), position(posiiton), rotation(rotation)
			, pivot(pivot), flip(flip)
		{}

		TextureId textureId = 0;
		Math::Rect sourceRect{};
		Math::Vector2 position = 0.0f;
		float rotation = 0.0f;
		Pivot pivot = Pivot::Center;
		Flip flip = Flip::None;
	};

	static std::unique_ptr<BatchRender> sBatchRenderer;

	std::vector<TextCommand> textCommands;
	std::vector<SpriteCommand> spriteCommands;
	Font font;
}

BatchRender* Angazi::Graphics::BatchRender::Get()
{
	ASSERT(sBatchRenderer != nullptr, "[BatchRenderer] Bacth Renderer not initialized.");
	return sBatchRenderer.get();
}

void BatchRender::Initialize()
{
	font.Initialize();
}

void BatchRender::Terminate()
{
	font.Terminate();
}

void BatchRender::Render()
{
	TextureId id = 0;
	Texture* texture = nullptr;

	// Sprites
	SpriteRenderer::Get()->BeginRender();
	for (const auto& command : spriteCommands)
	{
		if (id != command.textureId)
		{
			texture = TextureManager::Get()->GetTexture(command.textureId);
			id = command.textureId;
		}
		if (texture)
		{
			if (Math::IsEmpty(command.sourceRect))
			{
				SpriteRenderer::Get()->Draw(*texture, command.position, command.rotation, command.pivot, command.flip);
			}
			else
			{
				SpriteRenderer::Get()->Draw(*texture, command.sourceRect, command.position, command.rotation, command.pivot, command.flip);
			}
		}
	}
	spriteCommands.clear();
	SpriteRenderer::Get()->EndRender();

	for (const auto& command : textCommands)
		font.Draw(command.text.c_str(), command.size, command.x, command.y, command.color);
	textCommands.clear();
}

void BatchRender::AddScreenText(const char * str, float x, float y, float size, const Color & color)
{
	int len = (int)strlen(str);
	int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, str, len, NULL, 0);
	std::wstring wstr(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, str, len, &wstr[0], sizeNeeded);
	textCommands.emplace_back(std::move(wstr), size, x, y, color);
}

void BatchRender::AddSprite(TextureId textureId, const Math::Vector2 & position, Pivot pivot, Flip flip)
{
	spriteCommands.emplace_back(textureId, position, 0.0f, pivot, flip);
}

void BatchRender::AddSprite(TextureId textureId, const Math::Vector2 & position, float rotation, Pivot pivot, Flip flip)
{
	spriteCommands.emplace_back(textureId, position, rotation, pivot, flip);
}

void BatchRender::AddSprite(TextureId textureId, const Math::Rect & sourceRect, const Math::Vector2 & position, Pivot pivot, Flip flip)
{
	spriteCommands.emplace_back(textureId,sourceRect, position, 0.0f, pivot, flip);
}

void BatchRender::AddSprite(TextureId textureId, const Math::Rect & sourceRect, const Math::Vector2 & position, float rotation, Pivot pivot, Flip flip)
{
	spriteCommands.emplace_back(textureId, sourceRect, position, rotation, pivot, flip);
}

void BatchRender::StaticInitialize()
{
	ASSERT(sBatchRenderer == nullptr, "[BatchRenderer] Batch Renderer already initialized.");
	sBatchRenderer = std::make_unique<BatchRender>();

	sBatchRenderer->Initialize();
}

void BatchRender::StaticTerminate()
{
	if (sBatchRenderer != nullptr)
	{
		sBatchRenderer->Terminate();
		sBatchRenderer.reset();
	}
}
