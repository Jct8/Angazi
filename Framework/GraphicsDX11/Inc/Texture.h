#pragma once

#ifdef ENABLE_DIRECTX11

namespace Angazi::Graphics
{
	class Texture
	{
	public:
		Texture() = default;
		~Texture();

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		Texture(Texture&&) = default;

		void Initialize(const std::filesystem::path& fileName);
		void Initialize(const std::vector<std::filesystem::path>& cubeSides);
		void Terminate();

		void BindVS(uint32_t slot = 0) const;
		void BindPS(uint32_t slot = 0) const;
		void UnbindVS(uint32_t slot = 0) const;
		void UnbindPS(uint32_t slot = 0) const;

		ID3D11ShaderResourceView* GetShaderResourceView() { return mShaderResourceView; }

		uint32_t GetWidth() const { return mWidth; }
		uint32_t GetHeight() const { return mHeight; }

	private:
		friend class SpriteRenderer;

		ID3D11ShaderResourceView* mShaderResourceView = nullptr;
		uint32_t mWidth;
		uint32_t mHeight;
	};

} // namespace Angazi::Graphics

#endif