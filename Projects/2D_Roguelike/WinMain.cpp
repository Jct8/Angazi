#include "GameState.h"
#include <Angazi/Inc/Angazi.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Angazi::MainApp().AddState<GameState>("GameState");
	Angazi::MainApp().Run({ "Rougelike Dungeon" ,1080, 720 });
	return 0;
}