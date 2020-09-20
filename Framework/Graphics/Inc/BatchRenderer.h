#pragma once

namespace Angazi::Graphics
{
	class BatchRenderer
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static BatchRenderer* Get();
	public:

		void Initialize();
		void Terminate();

		void Render();

		void AddScreenText(const char* str, float x, float y, float size, const Color& color);
		void AddSprite(TextureId textureId, const Math::Vector2& position, Pivot pivot = Pivot::Center, Flip flip = Flip::None);
		void AddSprite(TextureId textureId, const Math::Vector2& position, float rotation, Pivot pivot = Pivot::Center, Flip flip = Flip::None);
		void AddSprite(TextureId textureId, const Math::Rect& sourceRect, const Math::Vector2& position, Pivot pivot = Pivot::Center, Flip flip = Flip::None);
		void AddSprite(TextureId textureId, const Math::Rect& sourceRect, const Math::Vector2& position, float rotation, Pivot pivot = Pivot::Center, Flip flip = Flip::None);

	private:

	};
}