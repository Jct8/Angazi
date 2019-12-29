#pragma once

namespace Angazi::Graphics
{
	class Shader
	{
	public:
		void Initialize(const std::filesystem::path filePath);
		void Terminate();

		void Bind();
	
	private:
		unsigned int CompileShader(unsigned int type, const std::string& source);
		std::vector<std::string> ParseShader(const std::filesystem::path& filepath);
		unsigned int mProgram;
	};

}