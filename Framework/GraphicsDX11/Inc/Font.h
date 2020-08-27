#pragma once

#ifdef ENABLE_DIRECTX11

interface IFW1Factory;
interface IFW1FontWrapper;

namespace Angazi::Graphics
{
	using Color = Angazi::Math::Vector4;
	class Font
	{
	public:
		Font();
		~Font();

		void Initialize();
		void Terminate();

		void Draw(const wchar_t* str, float x, float y, float size, Color color);
		void Draw(const wchar_t* str, const Math::Vector2& position, float size, Color color);

	private:

		IFW1Factory* mFW1Factory;
		IFW1FontWrapper* mFontWrapper;
	};
}

#endif