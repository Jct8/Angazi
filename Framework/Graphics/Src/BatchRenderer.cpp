#include "Precompiled.h"
#include "BatchRenderer.h"

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

	static std::unique_ptr<BatchRender> sBatchRenderer;

	std::vector<TextCommand> textCommands;
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
