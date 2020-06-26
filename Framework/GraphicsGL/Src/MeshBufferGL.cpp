#include "Precompiled.h"
#include "MeshBufferGL.h"
#ifdef ENABLE_OPENGL

#include "Graphics/Inc/VertexTypes.h"

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	struct VertexElementDesc
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;

		static unsigned int GetSizeOfType(unsigned int type)
		{
			switch (type)
			{
			case GL_FLOAT:			return 4;
			case GL_INT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
			}
			return(0);
		}
	};

	auto GetVertexLayout(uint32_t vertexFormat)
	{
		std::vector<VertexElementDesc> vertexLayout; // rvo - copy elision

		if (vertexFormat & VE_Position)
			vertexLayout.push_back({ GL_FLOAT,3,GL_FALSE });

		if (vertexFormat & VE_Normal)
			vertexLayout.push_back({ GL_FLOAT,3,GL_FALSE });

		if (vertexFormat & VE_Tangent)
			vertexLayout.push_back({ GL_FLOAT,3,GL_FALSE });

		if (vertexFormat & VE_Color)
			vertexLayout.push_back({ GL_FLOAT,4,GL_FALSE });

		if (vertexFormat & VE_TexCoord)
			vertexLayout.push_back({ GL_FLOAT,2,GL_FALSE });

		if (vertexFormat & VE_BlendIndex)
			vertexLayout.push_back({ GL_INT,4,GL_FALSE });

		if (vertexFormat & VE_BlendWeight)
			vertexLayout.push_back({ GL_FLOAT,4,GL_FALSE });


		return vertexLayout;
	}
}

MeshBuffer::~MeshBuffer()
{
	ASSERT(!glIsBuffer(mVertexBuffer), "[MeshBufferGL] Terminate() must be called to clean up!");
	ASSERT(!glIsBuffer(mIndexBuffer), "[MeshBufferGL] Terminate() must be called to clean up!");
	ASSERT(!glIsVertexArray(mVertexArray) , "[MeshBufferGL] Terminate() must be called to clean up!");
}

void MeshBuffer::SetTopology(Topology topology)
{
	if (topology == Topology::Lines)
		mTopology = GL_LINES;
	if (topology == Topology::Triangles)
		mTopology = GL_TRIANGLES;
	if (topology == Topology::TrianglesStrip)
		mTopology = GL_TRIANGLE_STRIP;
}

void MeshBuffer::Update(const void * vertexData, uint32_t numVertices)
{
	mVertexCount = numVertices;

	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	void *resource = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(resource, vertexData, mVertexCount * mVertexSize);
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void MeshBuffer::Terminate()
{
	mIndexCount = 0;
	mVertexSize = 0;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &mVertexArray);
}

void MeshBuffer::Draw() const
{
	if (mIndexCount > 0)
	{
		glBindVertexArray(mVertexArray);
		glDrawElements(mTopology, mIndexCount, GL_UNSIGNED_INT, nullptr);
	}
	else
	{
		glBindVertexArray(mVertexArray);
		glDrawArrays(mTopology, 0, mVertexCount);
	}
}

void MeshBuffer::InitializeInternal(const void * vertices, int vertexSize, int vertexCount, const uint32_t * indices, int indexCount, uint32_t vertexFormat, bool dynamic)
{
	mVertexSize = vertexSize;
	mVertexCount = vertexCount;
	mIndexCount = indexCount;

	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	unsigned int drawType = GL_STATIC_DRAW;
	if (dynamic)
		drawType = GL_DYNAMIC_DRAW;
	glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, vertices, drawType);
	
	auto vertexLayout = GetVertexLayout(vertexFormat);
	uint32_t offset = 0;
	for (size_t i = 0; i < vertexLayout.size(); i++)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, vertexLayout[i].count, vertexLayout[i].type, vertexLayout[i].normalized,
			vertexSize, (const void*)(offset * VertexElementDesc::GetSizeOfType(vertexLayout[i].type)) );
		offset += vertexLayout[i].count;
	}

	if (indexCount > 0)
	{
		glGenBuffers(1, &mIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}
	glBindVertexArray(0);
}

#endif