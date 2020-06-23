#pragma once

#ifdef ENABLE_OPENGL

#include "Graphics/Inc/Colors.h"

namespace Angazi::Graphics
{
	class GraphicsSystem
	{
	public:
		static void StaticInitialize(HWND window, bool fullscreen);
		static void StaticTerminate();
		static GraphicsSystem* Get();
	public:
		GraphicsSystem() = default;
		~GraphicsSystem();

		GraphicsSystem(const GraphicsSystem&) = delete;
		GraphicsSystem& operator=(const GraphicsSystem&) = delete;

		void Initialize(HWND window, bool fullscreen);
		void Terminate();

		void BeginRender();
		void EndRender();

		void ToggleFullscreen();
		void Resize(uint32_t width, uint32_t height);

		void ResetRenderTarget();
		void ResetViewport();

		void SetClearColor(Color clearColor) { mClearColor = clearColor; }
		void SetVSync(bool vSync) { mVSync = vSync ? 1 : 0; }

		uint32_t GetBackBufferWidth() const;
		uint32_t GetBackBufferHeight() const;

		uint32_t GetCurrentPipeline() const { return pipeline; }
	private:
		friend LRESULT CALLBACK GraphicsSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

		uint32_t pipeline;

		HDC hDeviceContext = NULL;
		HGLRC glRenderingContext = NULL;

		HWND myWindow;

		Color mClearColor = Colors::White;
		UINT mVSync = 1;
	};

}

#endif