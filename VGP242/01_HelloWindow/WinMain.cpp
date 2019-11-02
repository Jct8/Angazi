#include <Angazi/Inc/Angazi.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Angazi::AppConfig appConfig;
	appConfig.appName = "Hello Window";
	appConfig.windowWidth = 1280;
	appConfig.windowHeight = 720;

	Angazi::MainApp().Run(appConfig);
	return 0;
}

