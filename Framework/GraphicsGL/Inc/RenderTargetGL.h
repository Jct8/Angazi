#pragma once

#ifdef ENABLE_OPENGL

namespace Angazi::Graphics
{
	class RenderTarget
	{
	public:
		enum class Format
		{
			RGBA_U8,
			RGBA_F16,
			RGBA_U32,
			RGBA_F32,
			R_F16,
			R_S32
		};

		RenderTarget() = default;
		~RenderTarget();

		RenderTarget(const RenderTarget&) = delete;
		RenderTarget& operator=(const RenderTarget&) = delete;

		void Initialize(uint32_t width, uint32_t height, Format format);
		void Terminate();

		void BeginRender();
		void EndRender();

		void BindPS(uint32_t slot) const;
		void BindDepthPS(uint32_t slot) const;

		void UnbindPS(uint32_t slot) const;

		void* GetShaderResourceView() const { return reinterpret_cast<void*>(static_cast<intptr_t>(mShaderResource)); }
		uint32_t GetShaderResource() const { return mShaderResource; }

	private:
		uint32_t mRenderTarget;
		uint32_t mShaderResource;
		uint32_t mRenderBuffer;
		uint32_t mDepthStencil;

		uint32_t mViewportWidth;
		uint32_t mViewportHeight;
		uint32_t mViewportTopLeftX;
		uint32_t mViewportTopLeftY;
	};
}

#endif