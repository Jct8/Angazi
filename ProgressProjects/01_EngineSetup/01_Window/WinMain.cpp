#include <Angazi/Inc/Angazi.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Angazi::MainApp().AddState<Angazi::AppState>("DummyState");
	Angazi::MainApp().Run({ "Window" ,1280, 720 });
	return 0;
}