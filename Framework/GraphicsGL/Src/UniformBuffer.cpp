#include "Precompiled.h"
#include "UniformBuffer.h"

using namespace Angazi;
using namespace Angazi::GraphicsGL;

UniformBuffer::~UniformBuffer()
{
}

void UniformBuffer::Initialize(int size)
{
	glGenBuffers(1, &mUniformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, mUniformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::Terminate()
{
	glDeleteBuffers(GL_UNIFORM_BUFFER, &mUniformBuffer);
}

void UniformBuffer::Bind(uint32_t slot) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, mUniformBuffer);
}

void UniformBuffer::Update(const void * data) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, mUniformBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, mSize, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
