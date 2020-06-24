#include "Precompiled.h"
#include "VertexShaderGL.h"
#include "GraphicsSystemGL.h"

#ifdef ENABLE_OPENGL


using namespace Angazi;
using namespace Angazi::Graphics;

VertexShader::~VertexShader()
{
	ASSERT(!glIsProgram(mVertexShader), "[VertexShaderGL] Terminate() must be called to clean up!");
}

void VertexShader::Initialize(const std::filesystem::path& filePath, uint32_t vertextFormat, const char * shaderName)
{
	std::ifstream stream(filePath);
	ASSERT(stream, "[VertexShaderGL] - Error Reading File");

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
	mVertexShader = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &fileCstr);
	ASSERT(mVertexShader != 0, "[VertexShaderGL] - Error creating vertex shader");

	//mVertexShader = glCreateShader(GL_VERTEX_SHADER);
	//const char* src = shaderString.c_str();
	//
	//glShaderSource(mVertexShader, 1, &src, nullptr);
	//glCompileShader(mVertexShader);

	GLint result;
	glGetShaderiv(mVertexShader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(mVertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(mVertexShader, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(mVertexShader);
		ASSERT(result, "[VertexShaderGL] - Shader failed to compile - %s:", infoLog.data());
	}

	GLint isLinked = 0;
	glGetProgramiv(mVertexShader, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(mVertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(mVertexShader, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(mVertexShader);
		ASSERT(isLinked, "[VertexShaderGL] - Shader failed to link - %s", infoLog.data());
	}
}

void Angazi::Graphics::VertexShader::Terminate()
{
	glDeleteProgram(mVertexShader);
}

void Angazi::Graphics::VertexShader::Bind()
{
	glUseProgramStages(GraphicsSystem::Get()->GetCurrentPipeline(), GL_VERTEX_SHADER_BIT, mVertexShader);
}

#endif