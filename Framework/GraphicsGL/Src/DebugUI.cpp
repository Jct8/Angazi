#include "Precompiled.h"
#include "DebugUI.h"

#include <ImGui/Inc/imgui.h>
#include <ImGui/Inc/imgui_impl_opengl3.h>
#include <ImGui/Inc/imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

using namespace Angazi::Core;
using namespace Angazi::GraphicsGL;

namespace
{
	WindowMessageHandler sWindowMessageHandler;

	bool IsMouseInput(UINT message)
	{
		switch (message)
		{
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_LBUTTONDBLCLK:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_RBUTTONDBLCLK:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_MBUTTONDBLCLK:
			case WM_MOUSEWHEEL:
			case WM_MOUSEHWHEEL:
				return true;
		}
		return false;
	}

	bool IsKeyboardInput(UINT message)
	{
		switch (message)
		{
			case WM_KEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_CHAR:
				return true;
		}
		return false;
	}
}

LRESULT CALLBACK DebugUIGLMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	ImGuiIO& io = ImGui::GetIO();

	if (io.WantCaptureMouse && IsMouseInput(message))
		return ImGui_ImplWin32_WndProcHandler(window, message, wParam, lParam);

	if (io.WantCaptureKeyboard && IsKeyboardInput(message))
		return ImGui_ImplWin32_WndProcHandler(window, message, wParam, lParam);

	LRESULT result = ImGui_ImplWin32_WndProcHandler(window, message, wParam, lParam);
	if (result != 0)
		return result;

	return sWindowMessageHandler.ForwardMessage(window, message, wParam, lParam);
}

void DebugUIGL::StaticInitialize(HWND window, bool docking, bool multiViewport)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	if (docking)
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	if(multiViewport)
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplWin32_Init(window);
	const char* glslVersion = "#version 130";
	ImGui_ImplOpenGL3_Init(glslVersion);

	sWindowMessageHandler.Hook(window, DebugUIGLMessageHandler);

}

void DebugUIGL::StaticTerminate()
{
	sWindowMessageHandler.Unhook();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DebugUIGL::SetTheme(Theme theme)
{
	switch (theme)
	{
	case Theme::Classic:
		ImGui::StyleColorsClassic();
		break;
	case Theme::Dark:
		ImGui::StyleColorsDark();
		break;
	case Theme::Light:
		ImGui::StyleColorsLight();
		break;
	}
}

void DebugUIGL::BeginRender()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void DebugUIGL::EndRender()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

}