#pragma once

namespace Angazi::Graphics 
{
	class PixelShader
	{
	public:
		void Initialize(const std::filesystem::path& filePath);
		void Terminate();
		void Bind();

	private:
		ID3D11PixelShader *mPixelShader = nullptr;
	};


} //namespace Angazi::Graphics