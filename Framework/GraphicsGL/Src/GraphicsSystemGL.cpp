#include "Precompiled.h"
#include "GraphicsSystemGL.h"

#ifdef ENABLE_OPENGL

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	std::unique_ptr<GraphicsSystem> sGraphicsSystem;
	Core::WindowMessageHandler sWindowMessageHandler;
}

 //if window resize, update back buffer
LRESULT CALLBACK Graphics::GraphicsSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
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

void GraphicsSystem::StaticInitialize(HWND window, bool fullscreen)
{
	ASSERT(sGraphicsSystem == nullptr, "[GraphicsSystemGL] System already initialized!");
	sGraphicsSystem = std::make_unique<GraphicsSystem>();
	sGraphicsSystem->Initialize(window, fullscreen);
}

void GraphicsSystem::StaticTerminate()
{
	if (sGraphicsSystem != nullptr)
	{
		sGraphicsSystem->Terminate();
		sGraphicsSystem.reset();
	}
}

GraphicsSystem* GraphicsSystem::Get()
{
	ASSERT(sGraphicsSystem != nullptr, "[GraphicsSystemGL] No system registered.");
	return sGraphicsSystem.get();
}

GraphicsSystem::~GraphicsSystem()
{
	ASSERT(!glIsProgramPipeline(pipeline), "[GraphicsSystemGL] Terminate() must be called to clean up!");
}

void GraphicsSystem::Initialize(HWND window, bool fullscreen)
{
	myWindow = window;
	int pixelFormat;
	PIXELFORMATDESCRIPTOR pfd;
	RECT rect;
	GetClientRect(window, &rect);
	GLsizei width = rect.right - rect.left;
	GLsizei height = rect.bottom - rect.top;

	pfd.bReserved = 0;
	pfd.cAccumAlphaBits = 0;
	pfd.cAccumBits = 0;
	pfd.cAccumBlueBits = 24;
	pfd.cAccumGreenBits = 0;
	pfd.cAccumRedBits = 0;
	pfd.cAlphaBits = 0;
	pfd.cAlphaShift = 0;
	pfd.cAuxBuffers = 0;
	pfd.cBlueBits = 0;
	pfd.cBlueShift = 0;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 8;
	pfd.cGreenBits = 0;
	pfd.cGreenShift = 0;
	pfd.cRedBits = 0;
	pfd.cRedShift = 0;
	pfd.cStencilBits = 0;
	pfd.dwDamageMask = 0;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pfd.dwLayerMask = 0;
	pfd.dwVisibleMask = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// Create a GDI device context
	hDeviceContext = GetDC(window);
	ASSERT(hDeviceContext != NULL, "[GraphicsSystemGL] Can't get device context for window.");

	// Pick a pixel format
	pixelFormat = ChoosePixelFormat(hDeviceContext,&pfd);
	ASSERT(pixelFormat != NULL, "[GraphicsSystemGL] Can't choose pixel format.");

	// Set a pixel format
	pixelFormat = SetPixelFormat(hDeviceContext, pixelFormat, &pfd);
	ASSERT(pixelFormat != NULL, "[GraphicsSystemGL] Can't set pixel format.");

	// Create OpenGL context 
	glRenderingContext = wglCreateContext(hDeviceContext);
	ASSERT(glRenderingContext != NULL, "[GraphicsSystemGL] Can't create GL context");

	// Make the context active
	ASSERT(wglMakeCurrent(hDeviceContext, glRenderingContext), "[GraphicsSystemGL] Can't make current GL context");

	// Init Glew
	ASSERT(glewInit() == GLEW_OK, "Glew Init Failed");
	// Link OpenGL Extension functions
	ASSERT(glGetString(GL_EXTENSIONS),"Glew extentions failed");

	Resize(width, height);
	sWindowMessageHandler.Hook(window, GraphicsSystemMessageHandler);

	glEnable(GL_DEPTH_TEST);

	glGenProgramPipelines(1, &pipeline);
	glBindProgramPipeline(pipeline);

	LOG("OpenGL version supported by this platform: (%s)", glGetString(GL_VERSION));
	LOG("Vendor: (%s)", glGetString(GL_VENDOR));
	LOG("Renderor: (%s)", glGetString(GL_RENDERER));
}

void GraphicsSystem::Terminate()
{
	glDeleteProgramPipelines(1,&pipeline);

	// Restore original window's procedure
	sWindowMessageHandler.Unhook();

	ASSERT(wglMakeCurrent(NULL, NULL), "[GraphicsSystemGL] Release of DC and RC failed");
	ASSERT(wglDeleteContext(glRenderingContext), "[GraphicsSystemGL] Release of rendering context failed");
	ASSERT(ReleaseDC(myWindow,hDeviceContext), "[GraphicsSystemGL] Release of Device context failed");
}

void GraphicsSystem::BeginRender()
{
	wglMakeCurrent(hDeviceContext, glRenderingContext);
	glClearColor(mClearColor.x, mClearColor.y, mClearColor.z, mClearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

void GraphicsSystem::EndRender()
{
	SwapBuffers(hDeviceContext);
}

void GraphicsSystem::ToggleFullscreen()
{
	ASSERT(false, "Not Implemented Yet!");
	//BOOL fullscreen;
	//mSwapChain->GetFullscreenState(&fullscreen, nullptr);
	//mSwapChain->SetFullscreenState(!fullscreen, nullptr);
}

void GraphicsSystem::Resize(uint32_t width, uint32_t height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45.0, 1.0 * (width / height), 1.0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GraphicsSystem::ResetRenderTarget()
{
	ASSERT(false, "Not Implemented Yet!");
	//mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

void GraphicsSystem::ResetViewport()
{
	ASSERT(false, "Not Implemented Yet!");
	//mImmediateContext->RSSetViewports(1, &mViewport);
}

uint32_t GraphicsSystem::GetBackBufferWidth() const
{
	RECT rcCli;
	GetClientRect(WindowFromDC(hDeviceContext), &rcCli);
	return rcCli.right - rcCli.left;
}

uint32_t GraphicsSystem::GetBackBufferHeight() const
{
	RECT rcCli;
	GetClientRect(WindowFromDC(hDeviceContext), &rcCli);
	return rcCli.bottom - rcCli.top;
}

#endif