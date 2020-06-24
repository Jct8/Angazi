#pragma once
#ifdef ENABLE_OPENGL

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

		uint32_t GetWidth() const { return mWidth; }
		uint32_t GetHeight() const { return mHeight; }
	private:
		uint32_t mTextureID;
		uint32_t mWidth;
		uint32_t mHeight;
	};
} // namespace Angazi::GraphicsGL

#endif