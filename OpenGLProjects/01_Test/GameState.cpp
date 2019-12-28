#include "GameState.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{
	float positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	std::string vertexShader = 
		"#version 330 core\n"
		"\n"
		"layout(location =0) in vec4 position;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = position;\n"
		"}\n";
	std::string fragmentShader =
		"#version 330 core\n"
		"\n"
		"layout(location =0) out vec4 color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	color = vec4(1.0 , 0.0 , 0.0 , 1.0);\n"
		"}\n";

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER,buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	unsigned int program = glCreateProgram();

	unsigned int id = glCreateShader(GL_VERTEX_SHADER);
	const char* src = vertexShader.c_str();
	glShaderSource(id, 1, &src,nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = (char*) alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		glDeleteShader(id);
	}
	unsigned int vs = id;

	unsigned int idFrag = glCreateShader(GL_FRAGMENT_SHADER);
	const char* srcFrag = fragmentShader.c_str();
	glShaderSource(idFrag, 1, &srcFrag, nullptr);
	glCompileShader(idFrag);
	unsigned int fs = idFrag;

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	glUseProgram(program);
	
}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{

}

void GameState::Render()
{
	glDrawArrays(GL_TRIANGLES, 0, 3);
}