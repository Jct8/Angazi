#include "Precompiled.h"
#include "Shader.h"

#include <fstream>
#include <sstream>

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	const char * vsDefine = "#define COMPILING_VS\n";
	const char * fsDefine = "#define COMPILING_FS\n";
}

void Angazi::Graphics::Shader::Initialize(const std::filesystem::path filePath)
{
	mProgram = glCreateProgram();

	std::ifstream file(filePath.c_str());
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string fileContents = buffer.str();

	std::vector<std::string> shaders = ParseShader(filePath);

	std::string vsSource = shaders[0];
	std::string fsSource = shaders[1];

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vsSource);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fsSource);

	glAttachShader(mProgram, vs);
	glAttachShader(mProgram, fs);
	glLinkProgram(mProgram);
	glValidateProgram(mProgram);

	glDeleteShader(fs);
	glDeleteShader(vs);
}

void Angazi::Graphics::Shader::Terminate()
{
	glDeleteProgram(mProgram);
}

void Angazi::Graphics::Shader::Bind()
{
	glUseProgram(mProgram);
}

unsigned int Angazi::Graphics::Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		glDeleteShader(id);
		return 0;
	}

	return id;
}

std::vector<std::string> Angazi::Graphics::Shader::ParseShader(const std::filesystem::path & filepath)
{
	std::ifstream stream(filepath);
	std::vector<std::string> retVec;

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	retVec.push_back(ss[0].str());
	retVec.push_back(ss[1].str());
	
	return retVec;
}
