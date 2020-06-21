#include "Precompiled.h"
//#include "GraphicsSystem.h"
//
//using namespace Angazi;
//using namespace Angazi::Graphics;
//
//namespace
//{
//	std::unique_ptr<GraphicsSystem> sGraphicsSystem;
//	Core::WindowMessageHandler sWindowMessageHandler;
//
//	std::unique_ptr<GraphicsSystemDX11> sCurrentGraphicsSystem;
//}
//
//// if window resize, update back buffer
//LRESULT CALLBACK Graphics::GraphicsSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	if (sGraphicsSystem)
//	{
//		switch (message)
//		{
//			case WM_SIZE:
//			{
//				const uint32_t width = static_cast<uint32_t>(LOWORD(lParam));
//				const uint32_t height = static_cast<uint32_t>(HIWORD(lParam));
//				sGraphicsSystem->Resize(width, height);
//				break;
//			}
//		}
//	}
//	return sWindowMessageHandler.ForwardMessage(window, message, wParam, lParam);
//}
//
//void GraphicsSystem::StaticInitialize(HWND window, bool fullscreen)
//{
//	ASSERT(sGraphicsSystem == nullptr, "[GraphicsSystem] System already initialized!");
////#if ENABLE_DIRECTX
//	sGraphicsSystem = std::make_unique<GraphicsSystem>();
//	sCurrentGraphicsSystem = std::make_unique<GraphicsSystemDX11>();
////#else
//	//sGraphicsSystem = std::make_unique<GraphicsSystemGL>();
////#endif
//
//	sCurrentGraphicsSystem->Initialize(window, fullscreen);
//}
//
//void GraphicsSystem::StaticTerminate()
//{
//	if (sGraphicsSystem != nullptr)
//	{
//		sGraphicsSystem->Terminate();
//		sGraphicsSystem.reset();
//	}
//}
//
//GraphicsSystem* GraphicsSystem::Get()
//{
//	ASSERT(sGraphicsSystem != nullptr, "[GraphicsSystem] No system registered.");
//	return sGraphicsSystem.get();
//}
//
//GraphicsSystem::~GraphicsSystem()
//{
//}
//
//void GraphicsSystem::Initialize(HWND window, bool fullscreen)
//{
//	sCurrentGraphicsSystem->Initialize(window,fullscreen);
//
//	// Hook application to window's procedure
//	sWindowMessageHandler.Hook(window, GraphicsSystemMessageHandler);
//}
//
//void GraphicsSystem::Terminate()
//{
//	// Restore original window's procedure
//	sWindowMessageHandler.Unhook();
//
//	sCurrentGraphicsSystem->Terminate();
//}
//
//void GraphicsSystem::BeginRender()
//{
//	sCurrentGraphicsSystem->EndRender();
//}
//
//void GraphicsSystem::EndRender()
//{
//	sCurrentGraphicsSystem->EndRender();
//}
//
//void GraphicsSystem::ToggleFullscreen()
//{
//	sCurrentGraphicsSystem->ToggleFullscreen();
//}
//
//void GraphicsSystem::Resize(uint32_t width, uint32_t height)
//{
//	sCurrentGraphicsSystem->Resize(width,height);
//}
//
//void GraphicsSystem::ResetRenderTarget()
//{
//	sCurrentGraphicsSystem->ResetRenderTarget();
//}
//
//void GraphicsSystem::ResetViewport()
//{
//	sCurrentGraphicsSystem->ResetViewport();
//}
//
//void Angazi::Graphics::GraphicsSystem::SetClearColor(Color clearColor)
//{
//	sCurrentGraphicsSystem->SetClearColor(clearColor);
//}
//
//uint32_t GraphicsSystem::GetBackBufferWidth() const
//{
//	return sCurrentGraphicsSystem->GetBackBufferWidth();
//}
//
//uint32_t GraphicsSystem::GetBackBufferHeight() const
//{
//	return sCurrentGraphicsSystem->GetBackBufferHeight();
//}
//
//ID3D11Device * Angazi::Graphics::GraphicsSystem::GetDevice() const
//{
//	return sCurrentGraphicsSystem->GetDevice();
//}
//
//ID3D11DeviceContext * Angazi::Graphics::GraphicsSystem::GetContext() const
//{
//	return sCurrentGraphicsSystem->GetContext();
//}
