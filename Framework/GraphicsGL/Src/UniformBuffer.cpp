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
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	mSize = size;
}

void ConstantBuffer::Terminate()
{
	glDeleteBuffers(GL_UNIFORM_BUFFER, &mConstantBuffer);
}

void ConstantBuffer::BindVS(uint32_t slot) const
{
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, mConstantBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, mConstantBuffer);
}
void ConstantBuffer::BindPS(uint32_t slot) const
{
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, mConstantBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, mConstantBuffer);
}
void ConstantBuffer::UnbindVS(uint32_t slot) const
{
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, mConstantBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void ConstantBuffer::UnbindPS(uint32_t slot) const
{
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, mConstantBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ConstantBuffer::Update(const void * data) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, mConstantBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, mSize, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

#endif //  ENABLE_OPENGL
