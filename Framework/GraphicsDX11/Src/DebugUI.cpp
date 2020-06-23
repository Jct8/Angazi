#include "Precompiled.h"

#ifdef ENABLE_DIRECTX11
#include "DebugUI.h"

#include "D3DUtil.h"
#include <ImGui/Inc/imgui.h>
#include <ImGui/Inc/imgui_impl_dx11.h>
#include <ImGui/Inc/imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

using namespace Angazi::Core;
using namespace Angazi::Graphics;

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

	void CustomStyle()
	{
		ImGuiStyle & style = ImGui::GetStyle();
		ImVec4 * colors = style.Colors;

		/// 0 = FLAT APPEARENCE
		/// 1 = MORE "3D" LOOK
		int is3D = 1;

		colors[ImGuiCol_Text] = ImVec4(0.53f, 0.81f, 0.94f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.12f, 0.71f);
		colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.335f, 0.335f, 0.335f, 1.000f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.76f, 0.76f, 0.76f, 0.77f);
		colors[ImGuiCol_Separator] = ImVec4(0.000f, 0.000f, 0.000f, 0.137f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.700f, 0.671f, 0.600f, 0.290f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.702f, 0.671f, 0.600f, 0.674f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

		style.PopupRounding = 3;
		style.Alpha = 0.8f;
		style.WindowPadding = ImVec2(4, 4);
		style.FramePadding = ImVec2(6, 4);
		style.ItemSpacing = ImVec2(6, 2);

		style.ScrollbarSize = 18;

		style.WindowBorderSize = 1;
		style.ChildBorderSize = 1;
		style.PopupBorderSize = 1;
		style.FrameBorderSize = static_cast<float>(is3D);

		style.WindowRounding = 5.0f;
		style.ChildRounding = 3;
		style.FrameRounding = 3;
		style.ScrollbarRounding = 2;
		style.GrabRounding = 3;

#ifdef IMGUI_HAS_DOCK 
		style.TabBorderSize = static_cast<float>(is3D);
		style.TabRounding = 3;

		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 5.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
#endif
	}
}

LRESULT CALLBACK DebugUIMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
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

void DebugUI::StaticInitialize(HWND window, bool docking, bool multiViewport)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	if (docking)
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	if(multiViewport)
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(GetDevice(),GetContext());

	sWindowMessageHandler.Hook(window, DebugUIMessageHandler);

	DebugUI::SetTheme(DebugUI::Theme::Custom);
}

void DebugUI::StaticTerminate()
{
	sWindowMessageHandler.Unhook();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DebugUI::SetTheme(Theme theme)
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
	case Theme::Custom:
		CustomStyle();
		break;
	}
}

void DebugUI::BeginRender()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void DebugUI::EndRender()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

}

#endif