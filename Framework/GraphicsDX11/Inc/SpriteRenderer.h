#pragma once

#include "Graphics/Inc/SpriteTypes.h"
namespace DirectX { class CommonStates; class SpriteBatch; }

namespace Angazi::Graphics
{
	class Texture;

	class SpriteRenderer
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static SpriteRenderer* Get();
	public:
		SpriteRenderer() = default;
		~SpriteRenderer();

		SpriteRenderer(const SpriteRenderer&) = delete;
		SpriteRenderer& operator=(const SpriteRenderer&) = delete;

		void Initialize();
		void Terminate();

		void SetTransform(const Math::Matrix4& transform);

		void BeginRender();
		void EndRender();

		void Draw(const Texture& texture, const Math::Vector2& pos, float rotation = 0.0f, Pivot pivot = Pivot::Center, Flip flip = Flip::None);
		void Draw(const Texture& texture, const Math::Rect& sourceRect, const Math::Vector2& pos, float rotation = 0.0f, Pivot pivot = Pivot::Center, Flip flip = Flip::None);

	private:
		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
		std::unique_ptr<DirectX::CommonStates> mCommonStates;

		Math::Matrix4 mTransform;
	};
}