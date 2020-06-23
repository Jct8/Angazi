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
	ASSERT(mPixelShader != 0, "[PixelShaderGL] - Error creating pixel shader");

	/*mPixelShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* src = shaderString.c_str();

	glShaderSource(mPixelShader, 1, &src, nullptr);
	glCompileShader(mPixelShader);*/

	GLint result;
	glGetShaderiv(mPixelShader, GL_COMPILE_STATUS, &result);
	ASSERT(result, "Shader failed to compile.");
	if (result == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(mPixelShader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(mPixelShader, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(mPixelShader);
		ASSERT(result, "[PixelShaderGL] - Shader failed to compile - %s:", infoLog.data());
	}

	GLint isLinked = 0;
	glGetProgramiv(mPixelShader, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(mPixelShader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(mPixelShader, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(mPixelShader);
		ASSERT(isLinked, "[PixelShaderGL] - Shader failed to link - %s", infoLog.data());
	}
}

void Angazi::Graphics::PixelShader::Terminate()
{
	glDeleteProgram(mPixelShader);
}

void Angazi::Graphics::PixelShader::Bind()
{
	glUseProgramStages(GraphicsSystem::Get()->GetCurrentPipeline(), GL_FRAGMENT_SHADER_BIT, mPixelShader);
}

#endif