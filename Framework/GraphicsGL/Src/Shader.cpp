#include "Precompiled.h"
#include "Shader.h"
#ifdef ENABLE_OPENGL

using namespace Angazi;
using namespace Angazi::Graphics;

void Shader::Initialize(const std::filesystem::path filePath, const char * VSshaderName, const char * PSshaderName)
{
	mProgram = glCreateProgram();

	std::ifstream file(filePath.c_str());
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string fileContents = buffer.str();

	std::vector<std::string> shaders = ParseShader(filePath, VSshaderName, PSshaderName);
	std::string vsSource = shaders[0];
	std::string fsSource = shaders[1];

	uint32_t vs = CompileShader(GL_VERTEX_SHADER, vsSource);
	uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fsSource);

	glAttachShader(mProgram, vs);
	glAttachShader(mProgram, fs);
	glLinkProgram(mProgram);

	GLint isLinked = 0;
	glGetProgramiv(mProgram, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(mProgram, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(mProgram);
		glDeleteShader(vs);
		glDeleteShader(fs);
		ASSERT(isLinked, "[Shader] - Shader failed to link - %s", infoLog.data());
	}
	glDetachShader(mProgram, vs);
	glDetachShader(mProgram, fs);
}

void Shader::Terminate()
{
	glDeleteProgram(mProgram);
}

void Shader::Bind()
{
	glUseProgram(mProgram);
}

Shader::~Shader()
{
	ASSERT(!glIsProgram(mProgram), "[Shader] Terminate() must be called to clean up!");
}

void Shader::SetUniform1i(const std::string name, int value)
{
	glUniform1i(glGetUniformLocation(mProgram,name.c_str()), value);
}

void Shader::SetUniform1f(const std::string name, float value)
{
	glUniform1f(glGetUniformLocation(mProgram, name.c_str()), value);
}

void Shader::SetUniform3f(const std::string name, Math::Vector3 vec)
{
	glUniform3f(glGetUniformLocation(mProgram, name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::SetUniform4f(const std::string name, Math::Vector4 vec)
{
	glUniform4f(glGetUniformLocation(mProgram, name.c_str()), vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetUniform4f(const std::string name, float v0, float v1, float v2, float v3)
{
	glUniform4f(glGetUniformLocation(mProgram, name.c_str()), v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string name, Math::Matrix4 mat)
{
	GLfloat glMat[16] = {
	mat._11,mat._12, mat._13, mat._14,
	mat._21,mat._22, mat._23, mat._24,
	mat._31,mat._32, mat._33, mat._34,
	mat._41,mat._42, mat._43, mat._44
	};
	glUniformMatrix4fv(glGetUniformLocation(mProgram, name.c_str()), 1, GL_TRUE, glMat);
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	uint32_t id = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	ASSERT(result, "Shader failed to compile.");
	if (result == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(id, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(id);
		ASSERT(result, "[Shader] - Shader failed to compile - %s:", infoLog.data());
	}

	return id;
}

std::vector<std::string> Shader::ParseShader(const std::filesystem::path & filepath, const char * VSshaderName, const char * PSshaderName)
{
	std::ifstream stream(filepath);
	ASSERT(stream, "[Shader] - Error Reading File");
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
			if (line.find(VSshaderName) != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find(PSshaderName) != std::string::npos)
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

#endif