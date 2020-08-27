#pragma once

namespace Angazi::Graphics
{
	class BatchRender
	{
	public:
		static void StaticInitialize();
		static void StaticTerminate();
		static BatchRender* Get();
	public:

		void Initialize();
		void Terminate();

		void Render();

		void AddScreenText(const char* str, float x, float y, float size, const Color& color);

	private:

	};
}