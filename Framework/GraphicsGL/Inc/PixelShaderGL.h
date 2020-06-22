#pragma once
#ifdef ENABLE_OPENGL

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
		uint32_t GetId() { return mPixelShader; }

	private:
		uint32_t mPixelShader = 0;
	};


} //namespace Angazi::Graphics

#endif