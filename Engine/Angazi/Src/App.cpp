#include "Precompiled.h"
#include "App.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;

void Angazi::App::Run(AppConfig appConfig)
{
	mAppConfig = appConfig;

	// Setup out application window
	mWindow.Initialize(GetModuleHandle(NULL), appConfig.appName.c_str(), appConfig.windowWidth, appConfig.windowHeight);

	//Initialize the input systems
	auto handle = mWindow.GetWindowHandle();
	InputSystem::StaticInitialize(handle);

	//Initialize the graphics systems
	GraphicsSystem::StaticInitialize(handle, false);

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
	GraphicsSystem::StaticTerminate();
	InputSystem::StaticTerminate();

	//Terminate window
	mWindow.Terminate();
}