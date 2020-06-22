#include "Precompiled.h"
#include "PixelShaderGL.h"
#include "GraphicsGL.h"
#ifdef ENABLE_OPENGL

using namespace Angazi;
using namespace Angazi::Graphics;

PixelShader::~PixelShader()
{
	//ASSERT(mPixelShader == nullptr, "[PixelShader] Terminate() must be called to clean up!");
}

void PixelShader::Initialize(const std::filesystem::path& filePath, const char * shaderName)
{
	std::ifstream stream(filePath);
	ASSERT( stream , "[PixelShaderGL] - Error Reading File");

	std::string line;
	std::stringstream shaderSource;
	bool read = false;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find(shaderName) != std::string::npos)
				read = true;
			else
				read = false;
		}
		else if (read)
		{
			shaderSource << line << '\n';
		}
	}
	std::string shaderString = shaderSource.str();
	auto fileCstr = static_cast<const char *>(shaderString.c_str());
	mPixelShader = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fileCstr );


	//mPixelShader = glCreateShader(GL_FRAGMENT_SHADER);
	//const char* src = shaderString.c_str();
	//glShaderSource(mPixelShader, 1, &src, nullptr);
	//glCompileShader(mPixelShader);

	//int result;
	//glGetShaderiv(mPixelShader, GL_COMPILE_STATUS, &result);
	//ASSERT(result, "Shader failed to compile.");
	//if (result == GL_FALSE)
	//{
	//	int length;
	//	glGetShaderiv(mPixelShader, GL_INFO_LOG_LENGTH, &length);
	//	char *message = (char*)alloca(length * sizeof(char));
	//	glGetShaderInfoLog(mPixelShader, length, &length, message);
	//	glDeleteShader(mPixelShader);
	//}

	ASSERT(mPixelShader != 0, "[PixelShaderGL] - Error creating pixel shader");

}

void Angazi::Graphics::PixelShader::Terminate()
{
	glDeleteProgram(mPixelShader);
}

void Angazi::Graphics::PixelShader::Bind()
{
	glUseProgramStages(GraphicsSystem::Get()->pipeline, GL_FRAGMENT_SHADER_BIT, mPixelShader);
	glBindProgramPipeline(GraphicsSystem::Get()->pipeline);
}

#endif