#include "Precompiled.h"
#include "UniformBuffer.h"

#ifdef ENABLE_OPENGL


using namespace Angazi;
using namespace Angazi::Graphics;

ConstantBuffer::~ConstantBuffer()
{
}

void ConstantBuffer::Initialize(int size)
{
	glGenBuffers(1, &mConstantBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, mConstantBuffer);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	mSize = size;
}

void ConstantBuffer::Terminate()
{
	glDeleteBuffers(GL_UNIFORM_BUFFER, &mConstantBuffer);
}

void ConstantBuffer::Bind(uint32_t slot) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, mConstantBuffer);
}

void ConstantBuffer::Update(const void * data) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, mConstantBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, mSize, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

#endif //  ENABLE_OPENGL
