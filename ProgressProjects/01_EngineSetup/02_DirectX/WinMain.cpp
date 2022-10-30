#include "TestStates.h"
#include <Angazi/Inc/Angazi.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Angazi::MainApp().AddState<RedState>("RedState");
	Angazi::MainApp().AddState<BlueState>("BlueState");
	Angazi::MainApp().Run({ "DirectX" ,1280, 720});
	return 0;
}