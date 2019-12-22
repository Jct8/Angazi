#include "GameState.h"
#include <Angazi/Inc/Angazi.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Angazi::MainApp().AddState<GameState>("GameState");
	Angazi::MainApp().Run({ "Hello Solar System" ,1280, 720});
	return 0;
}