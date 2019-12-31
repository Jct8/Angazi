#pragma once
#include "Colors.h"

namespace Angazi::GraphicsGL {

class GraphicsSystemGL
{
public:
	static void StaticInitialize(HWND window, bool fullscreen);
	static void StaticTerminate(HWND window);
	static GraphicsSystemGL* Get();

public:
	GraphicsSystemGL() = default;
	~GraphicsSystemGL();

	GraphicsSystemGL(const GraphicsSystemGL&) = delete;
	GraphicsSystemGL& operator=(const GraphicsSystemGL&) = delete;

	void Initialize(HWND window, bool fullscreen);
	void Terminate(HWND window);

	void BeginRender();
	void EndRender();

	uint32_t GetBackBufferWidth() const;
	uint32_t GetBackBufferHeight() const;


private:
	friend LRESULT CALLBACK GraphicsSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
	void Resize(uint32_t width, uint32_t height);

	Color mClearColor = Colors::White;
	UINT mVSync = 1;
	HDC hDeviceContext = NULL;
	HGLRC glRenderingContext = NULL ;
};

} // namespace Angazi::GraphicsGL