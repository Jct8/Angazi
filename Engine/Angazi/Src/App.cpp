#include "Precompiled.h"
#include "App.h"

using namespace Angazi;
using namespace Angazi::Graphics;
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
	GraphicsSystem::StaticInitialize(handle, false);
	DebugUI::StaticInitialize(handle, false, true);
	SimpleDraw::StaticInitialize(1024 * 1024);
#ifdef ENABLE_DIRECTX11
	Graphics::SpriteRenderer::StaticInitialize();
#endif

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

		Core::TimeUtil::Update();

		if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			Quit();
			continue;
		}

		//float deltaTime = Math::Min(1.0f / 60.0f,Core::TimeUtil::GetDeltaTime());
		float deltaTime = 1.0f / 60.0f;
		mCurrentState->Update(deltaTime);

		auto graphicsSystem = GraphicsSystem::Get();
		graphicsSystem->BeginRender();

		//Graphics::SpriteRenderer::Get()->BeginRender();
		mCurrentState->Render();
		//Graphics::SpriteRenderer::Get()->EndRender();

		DebugUI::BeginRender();
		mCurrentState->DebugUI();
		DebugUI::EndRender();

		graphicsSystem->EndRender();
	}

	mCurrentState->Terminate();

	//Terminate engine systems
#ifdef ENABLE_DIRECTX11
	Graphics::SpriteRenderer::StaticTerminate();
#endif
	SimpleDraw::StaticTerminate();
	DebugUI::StaticTerminate();
	GraphicsSystem::StaticTerminate();

	InputSystem::StaticTerminate();

	//Terminate window
	mWindow.Terminate();
}

bool Angazi::App::OpenFileDialog(char fileName[MAX_PATH], const char * title, const char * filter)
{
	OPENFILENAMEA ofn = {};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = mWindow.GetWindowHandle();
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = title;
	return GetOpenFileNameA(&ofn);
}

bool Angazi::App::SaveFileDialog(char fileName[MAX_PATH], const char * title, const char * filter)
{
	OPENFILENAMEA ofn = {};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = mWindow.GetWindowHandle();
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = title;
	return GetSaveFileNameA(&ofn);
}

float App::GetTime()
{
	return Core::TimeUtil::GetTime();
}