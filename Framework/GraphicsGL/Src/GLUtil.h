#pragma once
#ifdef ENABLE_OPENGL

namespace Angazi::Graphics
{
	HDC GetDevice();
	HGLRC GetContext();

}//namespace Angazi::Graphics

#endif