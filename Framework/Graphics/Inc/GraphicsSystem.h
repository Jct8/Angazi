#pragma once
//#include "Colors.h"
//
//namespace Angazi::Graphics {
//
//class GraphicsSystem
//{
//public:
//	static void StaticInitialize(HWND window, bool fullscreen);
//	static void StaticTerminate();
//	static GraphicsSystem* Get();
//
//public:
//	GraphicsSystem() = default;
//	~GraphicsSystem();
//
//	GraphicsSystem(const GraphicsSystem&) = delete;
//	GraphicsSystem& operator=(const GraphicsSystem&) = delete;
//
//	void Initialize(HWND window, bool fullscreen);
//	void Terminate();
//
//	void BeginRender();
//	void EndRender();
//
//	void ToggleFullscreen();
//	void Resize(uint32_t width, uint32_t height);
//
//	void ResetRenderTarget();
//	void ResetViewport();
//
//	void SetClearColor(Color clearColor);
//	void SetVSync(bool vSync) { mVSync = vSync ? 1 : 0; }
//
//	uint32_t GetBackBufferWidth() const;
//	uint32_t GetBackBufferHeight() const;
//
//	////TODO - Remove these eventually
//	//ID3D11Device* GetDevice() const;
//	//ID3D11DeviceContext* GetContext() const;
//
//private:
//	friend LRESULT CALLBACK GraphicsSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
//
//	//friend ID3D11Device* GetDevice();
//	//friend ID3D11DeviceContext* GetContext();
//
//	UINT mVSync = 1;
//};
//
//} // namespace Angazi::Graphics