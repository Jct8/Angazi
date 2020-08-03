#pragma once

// Engine headers
#include <Core/Inc/Core.h>
#include <Math/Inc/EngineMath.h>

#ifdef ENABLE_OPENGL
// OpenGL headers
#define GLEW_STATIC
#include <Glew/include/GL/glew.h>

// OpenGL libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#endif