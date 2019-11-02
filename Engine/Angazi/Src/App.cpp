#include "Precompiled.h"
#include "App.h"

using namespace Angazi;

void Angazi::App::Run(AppConfig appConfig)
{
	mAppConfig = appConfig;

	// Setup out application window
	mWindow.Initialize(GetModuleHandle(NULL), appConfig.appName.c_str(), appConfig.windowWidth, appConfig.windowHeight);

	//Initialize engine systems

	mRunning = true;
	while (mRunning && mWindow.IsActive())
	{
		mWindow.ProcessMessage();

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			Quit();
		}
		//Do game stuff...
	}

	//Terminate engine systems

	//Terminate window
	mWindow.Terminate();
}

