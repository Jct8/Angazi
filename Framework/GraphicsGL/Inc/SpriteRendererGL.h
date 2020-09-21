#pragma once

#ifdef ENABLE_OPENGL

#include "Graphics/Inc/SpriteTypes.h"

#include "MeshBufferGL.h"
#include "PixelShaderGL.h"
#include "VertexShaderGL.h"
#include "UniformBuffer.h"
#include "BlendStateGL.h"

namespace Angazi::Graphics
{
	//class MeshBuffer;
	//class PixelShader;
	class Texture;
	//class VertexShader;
	//class ConstantBuffer;

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

		Graphics::BlendState mBlendState;
		Graphics::ConstantBuffer mConstantBuffer;
		Graphics::PixelShader mPixelShader;
		Graphics::VertexShader mVertexShader;
		Graphics::MeshBuffer mQuadMeshBuffer;
		Math::Matrix4 mTransform;
	};
}
#endif