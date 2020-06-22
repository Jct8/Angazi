#pragma once
#ifdef ENABLE_OPENGL

namespace Angazi::Graphics
{
	class VertexShader
	{
	public:
		VertexShader() = default;
		~VertexShader();

		VertexShader(const VertexShader&) = delete;
		VertexShader& operator=(const VertexShader&) = delete;

		void Initialize(const std::filesystem::path& filePath, uint32_t vertextFormat, const char * shaderName = "VS");
		void Terminate();
		void Bind();

		uint32_t GetId() { return mVertexShader; }

	private:
		uint32_t mVertexShader = 0;
	};


} //namespace Angazi::Graphics

#endif