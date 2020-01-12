#include "Precompiled.h"
#include "App.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::GraphicsGL;
using namespace Angazi::Input;

void Angazi::App::ChangeState(const std::string& name)
{
	if (auto iter = mAppStates.find(name); iter != mAppStates.end())
		mNextState = iter->second.get();
}

void Angazi::App::Run(AppConfig appConfig)
{
	mAppConfig = std::move(appConfig);

	// Setup out application window
	mWindow.Initialize(GetModuleHandle(NULL),
		mAppConfig.appName.c_str(),
		mAppConfig.windowWidth,
		mAppConfig.windowHeight);

	//Initialize the input systems
	auto handle = mWindow.GetWindowHandle();
	InputSystem::StaticInitialize(handle);

	//Initialize the graphics systems
	if (appConfig.api == GraphicsAPI::DirectX)
	{
		GraphicsSystem::StaticInitialize(handle, false);
		DebugUI::StaticInitialize(handle, false, true);
	}
	else if (appConfig.api == GraphicsAPI::OpenGL)
		GraphicsSystemGL::StaticInitialize(handle, false);



	//Initialize the starting state
	mCurrentState = mAppStates.begin()->second.get();
	mCurrentState->Initialize();

	mRunning = true;
	while (mRunning)
	{
		mWindow.ProcessMessage();
		if (!mWindow.IsActive())
		{
			Quit();
			continue;
		}

		if (mNextState)
		{
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();
		}
		auto inputSystem = InputSystem::Get();
		inputSystem->Update();

		if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			Quit();
			continue;
		}

		float deltaTime = 1.0f / 60.0f;
		mCurrentState->Update(deltaTime);


		if (appConfig.api == GraphicsAPI::DirectX)
		{
			auto graphicsSystem = GraphicsSystem::Get();
			graphicsSystem->BeginRender();

			mCurrentState->Render();

			DebugUI::BeginRender();
			mCurrentState->DebugUI();
			DebugUI::EndRender();

			graphicsSystem->EndRender();
		}
		else if (appConfig.api == GraphicsAPI::OpenGL)
		{
			auto graphicsSystem = GraphicsSystemGL::Get();
			graphicsSystem->BeginRender();

			mCurrentState->Render();

			graphicsSystem->EndRender();
		}



	}

	mCurrentState->Terminate();

	//Terminate engine systems
	if (appConfig.api == GraphicsAPI::DirectX)
	{
		DebugUI::StaticTerminate();
		GraphicsSystem::StaticTerminate();
	}
	else if (appConfig.api == GraphicsAPI::OpenGL)
		GraphicsSystemGL::StaticTerminate(handle);
	InputSystem::StaticTerminate();

	//Terminate window
	mWindow.Terminate();
}