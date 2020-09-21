#pragma once

#ifdef ENABLE_OPENGL

#include "BlendStateGL.h"

namespace Angazi::Graphics
{
	using Color = Angazi::Math::Vector4;
	class Font
	{
	public:
		Font() = default;
		~Font();

		void Initialize();
		void Terminate();

		void Draw(const wchar_t* str, float x, float y, float size, Color color);
		void Draw(const wchar_t* str, const Math::Vector2& position, float size, Color color);

	private:

		Graphics::BlendState mBlendState;
		int fontNormal = 0;
	};
}

#endif