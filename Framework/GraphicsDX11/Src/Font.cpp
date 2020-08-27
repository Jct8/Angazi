#include "Precompiled.h"
#include "Font.h"

#ifdef ENABLE_DIRECTX11

#include "D3DUtil.h"
#include "Graphics/Inc/Colors.h"
#include <FW1FontWrapper/Inc/FW1FontWrapper.h>

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	uint32_t ToColor(const Angazi::Graphics::Color& color)
	{
		uint8_t r = (uint8_t)(color.r * 255);
		uint8_t g = (uint8_t)(color.g * 255);
		uint8_t b = (uint8_t)(color.b * 255);
		return 0xff000000 | (b << 16) | (g << 8) | r;
	}
}
Font::Font()
	: mFW1Factory(nullptr)
	, mFontWrapper(nullptr)
{
}

Font::~Font()
{
}


void Font::Initialize()
{
	auto device = GetDevice();
	FW1CreateFactory(FW1_VERSION, &mFW1Factory);
	auto hr = mFW1Factory->CreateFontWrapper(device, L"Consolas", &mFontWrapper);
	ASSERT(SUCCEEDED(hr), "[Font] Failed to initialize FW1FontWrapper. Error = %x", hr);
}

void Font::Terminate()
{
	SafeRelease(mFontWrapper);
	SafeRelease(mFW1Factory);
}

void Font::Draw(const wchar_t * str, float x, float y, float size, Color color)
{
	auto context = GetContext();
	mFontWrapper->DrawString(context, str, x, y, size, ToColor(color), FW1_RESTORESTATE);
}

void Font::Draw(const wchar_t * str, const Math::Vector2& position, float size, Color color)
{
	Draw(str,position.x,position.y,size,color);
}

#endif
