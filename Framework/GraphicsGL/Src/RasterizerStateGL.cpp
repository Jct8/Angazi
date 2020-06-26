#include "Precompiled.h"
#include "RasterizerStateGL.h"

#ifdef ENABLE_OPENGL

#include "GraphicsSystemGL.h"

using namespace Angazi::Graphics;

namespace
{
	inline uint32_t GetCullMode(RasterizerState::CullMode cullMode)
	{
		switch (cullMode)
		{
		case RasterizerState::CullMode::Front:
			return GL_FRONT;
		case RasterizerState::CullMode::Back:
			return GL_BACK;
		case RasterizerState::CullMode::None:
			return GL_NONE;
		}
		return GL_NONE;
	};

	inline uint32_t GetFillMode(RasterizerState::FillMode fillMode)
	{
		switch (fillMode)
		{
		case RasterizerState::FillMode::Solid:
			return GL_FILL;
		case RasterizerState::FillMode::Wireframe:
			return GL_LINE;
		}
		return GL_LINE;
	};
}

RasterizerState::~RasterizerState()
{
	//ASSERT(mRasterizerState == nullptr, "RasterizerState -- Rasterizer state not released!");
}

void RasterizerState::Initialize(CullMode cullMode, FillMode fillMode)
{
	mCullMode = cullMode;
	mFillMode = fillMode;
}

void RasterizerState::Terminate()
{

}

void RasterizerState::Set()
{
	glCullFace(GetCullMode(mCullMode));
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GetFillMode(mFillMode));
}

void RasterizerState::Clear()
{
	glCullFace(GetCullMode(CullMode::None));
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GetFillMode(FillMode::Solid));
}

#endif