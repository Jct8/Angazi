#include "EditState.h"
#include <Angazi/Inc/Angazi.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Angazi::MainApp().AddState<EditState>("EditState");
	Angazi::MainApp().Run({ "Angazi Editor" ,1280, 720,false});
	return 0;
}