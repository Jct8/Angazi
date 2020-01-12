#include "Precompiled.h"
#include "GraphicsSystemGL.h"

using namespace Angazi;
using namespace Angazi::GraphicsGL;

namespace
{
	std::unique_ptr<GraphicsSystemGL> sGraphicsSystem;
	Core::WindowMessageHandler sWindowMessageHandler;
}

 //if window resize, update back buffer
LRESULT CALLBACK GraphicsGL::GraphicsSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (sGraphicsSystem)
	{
		switch (message)
		{
			case WM_SIZE:
			{
				const uint32_t width = static_cast<uint32_t>(LOWORD(lParam));
				const uint32_t height = static_cast<uint32_t>(HIWORD(lParam));
				sGraphicsSystem->Resize(width, height);
				break;
			}
		}
	}
	return sWindowMessageHandler.ForwardMessage(window, message, wParam, lParam);
}

void GraphicsSystemGL::StaticInitialize(HWND window, bool fullscreen)
{
	ASSERT(sGraphicsSystem == nullptr, "[GraphicsSystemGL] System already initialized!");
	sGraphicsSystem = std::make_unique<GraphicsSystemGL>();
	sGraphicsSystem->Initialize(window, fullscreen);
}

void GraphicsSystemGL::StaticTerminate(HWND window)
{
	if (sGraphicsSystem != nullptr)
	{
		sGraphicsSystem->Terminate(window);
		sGraphicsSystem.reset();
	}
}

GraphicsSystemGL* GraphicsSystemGL::Get()
{
	ASSERT(sGraphicsSystem != nullptr, "[GraphicsSystemGL] No system registered.");
	return sGraphicsSystem.get();
}

GraphicsSystemGL::~GraphicsSystemGL()
{
	
}

void GraphicsSystemGL::Initialize(HWND window, bool fullscreen)
{
	int pixelFormat;
	PIXELFORMATDESCRIPTOR pfd;
	RECT rect;
	GetClientRect(window, &rect);
	GLsizei width = rect.right - rect.left;
	GLsizei height = rect.bottom - rect.top;

	pfd.bReserved = 0;
	pfd.cAccumAlphaBits = 0;
	pfd.cAccumBits = 0;
	pfd.cAccumBlueBits = 0;
	pfd.cAccumGreenBits = 0;
	pfd.cAccumRedBits = 0;
	pfd.cAlphaBits = 0;
	pfd.cAlphaShift = 0;
	pfd.cAuxBuffers = 0;
	pfd.cBlueBits = 0;
	pfd.cBlueShift = 0;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.cGreenBits = 0;
	pfd.cGreenShift = 0;
	pfd.cRedBits = 0;
	pfd.cRedShift = 0;
	pfd.cStencilBits = 0;
	pfd.dwDamageMask = 0;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pfd.dwLayerMask = 0;
	pfd.dwVisibleMask = 0;
	pfd.iLayerType = 0;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	//create a GDI device context
	hDeviceContext = GetDC(window);
	ASSERT(hDeviceContext != NULL, "[GraphicsSystemGL] Can't get device context for window.");

	//pick a pixel format
	pixelFormat = ChoosePixelFormat(hDeviceContext,&pfd);
	ASSERT(pixelFormat != NULL, "[GraphicsSystemGL] Can't choose pixel format.");

	//set a pixel format
	pixelFormat = SetPixelFormat(hDeviceContext, pixelFormat, &pfd);
	ASSERT(pixelFormat != NULL, "[GraphicsSystemGL] Can't set pixel format.");

	//create OpenGL context 
	glRenderingContext = wglCreateContext(hDeviceContext);
	ASSERT(glRenderingContext != NULL, "[GraphicsSystemGL] Can't create GL context");

	//make the context active
	ASSERT(wglMakeCurrent(hDeviceContext, glRenderingContext), "[GraphicsSystemGL] Can't make current GL context");

	//Init Glew
	ASSERT(glewInit() == GLEW_OK, "Glew Init Failed");
	//Link OpenGL Extension functions
	ASSERT(glGetString(GL_EXTENSIONS),"Glew extentions failed");

	Resize(width, height);
	sWindowMessageHandler.Hook(window, GraphicsSystemMessageHandler);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
}

void GraphicsSystemGL::Terminate(HWND window)
{
	ASSERT(wglMakeCurrent(NULL, NULL), "[GraphicsSystemGL] Release of DC and RC failed");
	ASSERT(wglDeleteContext(glRenderingContext), "[GraphicsSystemGL] Release of rendering context failed");
	ASSERT(ReleaseDC(window,hDeviceContext), "[GraphicsSystemGL] Release of Device context failed");

	// Restore original window's procedure
	sWindowMessageHandler.Unhook();
}

void GraphicsSystemGL::BeginRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

void GraphicsSystemGL::EndRender()
{
	SwapBuffers(hDeviceContext);
}

uint32_t Angazi::GraphicsGL::GraphicsSystemGL::GetBackBufferWidth() const
{
	RECT rcCli;
	GetClientRect(WindowFromDC(hDeviceContext), &rcCli);
	return rcCli.right - rcCli.left;
}

uint32_t Angazi::GraphicsGL::GraphicsSystemGL::GetBackBufferHeight() const
{
	RECT rcCli;
	GetClientRect(WindowFromDC(hDeviceContext), &rcCli);
	return rcCli.bottom - rcCli.top;
}

void GraphicsSystemGL::Resize(uint32_t width, uint32_t height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45.0, 1.0 * (width / height), 1.0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}