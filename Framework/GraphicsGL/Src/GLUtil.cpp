#include "Precompiled.h"
#include "GLUtil.h"

#ifdef ENABLE_OPENGL
#include "GraphicsSystemGL.h"

HDC Angazi::Graphics::GetDevice()
{
	return GraphicsSystem::Get()->hDeviceContext;
}

HGLRC Angazi::Graphics::GetContext()
{
	return  GraphicsSystem::Get()->glRenderingContext;
}

#endif