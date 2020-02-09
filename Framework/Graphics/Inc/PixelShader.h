#pragma once

namespace Angazi::Graphics 
{
	class PixelShader
	{
	public:
		PixelShader() = default;
		~PixelShader();

		PixelShader(const PixelShader&) = delete;
		PixelShader& operator=(const PixelShader&) = delete;

		void Initialize(const std::filesystem::path& filePath, const char * shaderName = "PS");
		void Terminate();
		void Bind();

	private:
		ID3D11PixelShader *mPixelShader = nullptr;
	};


} //namespace Angazi::Graphics