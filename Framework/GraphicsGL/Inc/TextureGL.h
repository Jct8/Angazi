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

		void Initialize(const std::filesystem::path& fileName);
		void Initialize(const std::vector<std::filesystem::path>& cubeSides);
		void Terminate();

		void BindVS(uint32_t slot = 0) const;
		void BindPS(uint32_t slot = 0) const;
		void UnbindVS(uint32_t slot = 0) const;
		void UnbindPS(uint32_t slot = 0) const;

		//void Bind(const std::string &name,unsigned int slot = 0) const;
	private:
		unsigned int mTextureID;
	};
} // namespace Angazi::GraphicsGL

#endif