#include "GameState.h"
#include <Angazi/Inc/Angazi.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Angazi::MainApp().AddState<GameState>("GameState");
	Angazi::MainApp().Run({ "OpenGL SimpleDraw" ,1280, 720, Angazi::GraphicsAPI::OpenGL});
	return 0;
}