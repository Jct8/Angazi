#pragma once
#ifndef GLEW_STATIC
	#define GLEW_STATIC
#endif

// Engine headers
#include <Core/Inc/Core.h>
#include <Math/Inc/EngineMath.h>

#include <fstream>
#include <sstream>

// OpenGL headers
#include <Glew/include/GL/glew.h>
//#include <glatter/include/glatter/glatter.h>

// OpenGL libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")