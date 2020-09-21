#include "Precompiled.h"
#include "FontGL.h"

#ifdef ENABLE_OPENGL

#include "Graphics/Inc/Colors.h"

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
}

Font::~Font()
{
	//ASSERT(fs == nullptr, "FontGL -- Terminate() must be called to clean up!");
}

void Font::Initialize()
{
	mBlendState.Initialize(BlendState::Mode::AlphaBlend);
}

void Font::Terminate()
{
	mBlendState.Terminate();
}

void Font::Draw(const wchar_t* str, float x, float y, float size, Color color)
{
	mBlendState.Bind();
	mBlendState.ClearState();
}

void Font::Draw(const wchar_t* str, const Math::Vector2& position, float size, Color color)
{
	Draw(str, position.x, position.y, size, color);
}

#endif
