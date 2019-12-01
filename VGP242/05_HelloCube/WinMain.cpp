#include "GameState.h"
#include <Angazi/Inc/Angazi.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Angazi::MainApp().AddState<GameState>("RedState");
	Angazi::MainApp().Run({ "Hello Cube" ,1280, 720});
	return 0;
}