#pragma once

#include "ImGui/Inc/imgui.h"

namespace Angazi
{
	class EditorLogView
	{
	public:
		EditorLogView();

		void Clear();
		void AddLog(const char* fmt, ...);
		void Draw(const char* title, bool* p_opened = NULL);

	private:
		ImGuiTextBuffer Buf;
		ImGuiTextFilter Filter;
		ImVector<int> LineOffsets;		// Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
		bool AutoScroll;				// Keep scrolling if already at the bottom
	};

}