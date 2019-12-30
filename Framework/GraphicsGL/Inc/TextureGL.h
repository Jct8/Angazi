#pragma once

namespace Angazi::Graphics 
{
	class TextureGL
	{
	public:
		void Initialize(const std::filesystem::path filePath);
		void Terminate();

		void Bind(unsigned int slot = 0) const;
	private:
		unsigned int mTextureID;
	};

} // namespace Angazi::Graphics