#include "Precompiled.h"
#include "DepthStencilStateGL.h"

#ifdef ENABLE_OPENGL

#include "GraphicsSystemGL.h"

using namespace Angazi::Graphics;

DepthStencilState::~DepthStencilState()
{
	//ASSERT(mDepthStencilState == nullptr, "DepthStencilState -- DepthStencil state not released!");
}

void DepthStencilState::Initialize(bool depthEnable, bool depthWrite)
{
	mDepthEnabled = depthEnable;
	mDepthWrite = depthWrite;
}

void DepthStencilState::Terminate()
{
	Clear();
}

void DepthStencilState::Set()
{
	mDepthEnabled == true ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	mDepthWrite == true ? glDepthMask(GL_TRUE): glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);
}

void DepthStencilState::Clear()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}

#endif