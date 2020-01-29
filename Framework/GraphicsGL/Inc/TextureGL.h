#pragma once

namespace Angazi::GraphicsGL
{
	class TextureGL
	{
	public:
		void Initialize(const std::filesystem::path filePath, bool flip = false);
		void Terminate();

		void Bind(unsigned int slot = 0) const;
		void Bind(const std::string &name,unsigned int slot = 0) const;
	private:
		unsigned int mTextureID;
	};
} // namespace Angazi::GraphicsGL