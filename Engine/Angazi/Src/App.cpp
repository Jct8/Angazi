#include "Precompiled.h"
#include "App.h"

#include "MetaRegistration.h"

using namespace Angazi;
using namespace Angazi::Core;
using namespace Angazi::Graphics;
using namespace Angazi::Input;

void Angazi::App::ChangeState(const std::string& name)
{
	if (auto iter = mAppStates.find(name); iter != mAppStates.end())
		mNextState = iter->second.get();
}

void Angazi::App::Run(AppConfig appConfig)
{
	LOG("App -- Running ... ");

	mAppConfig = std::move(appConfig);

	LOG("App -- Registering meta types ... ");
	Core::StaticMetaRegister();
	Math::StaticMetaRegister();
	Graphics::StaticMetaRegister();
	Angazi::StaticMetaRegister();

	// Initialize timer
	TimeUtil::GetTime();

	// Setup out application window
	LOG("App -- Creating window ... ");
	mWindow.Initialize(GetModuleHandle(NULL), mAppConfig.appName.c_str(), mAppConfig.windowWidth, mAppConfig.windowHeight);

	auto handle = mWindow.GetWindowHandle();

	//Initialize the graphics systems
	GraphicsSystem::StaticInitialize(handle, false);
	TextureManager::StaticInitialize(Angazi::Core::FilePath::GetAssetFilePath() + "Images/");
	BatchRenderer::StaticInitialize();
	SpriteRenderer::StaticInitialize();
	ModelManager::StaticInitialize(Angazi::Core::FilePath::GetAssetFilePath() + "Models/");
	MeshManager::StaticInitialize(Angazi::Core::FilePath::GetAssetFilePath() + "Models/");

	// Start Logo 
	float startTimer = TimeUtil::GetTime() + 2.0f;
	TextureId startTextureId;
	startTextureId = TextureManager::Get()->Load("AngaziLogo.png", false, true);
	BatchRenderer::Get()->AddSprite(startTextureId, { mAppConfig.windowWidth * 0.5f , mAppConfig.windowHeight * 0.5f });
	GraphicsSystem::Get()->SetClearColor(Colors::Black);
	GraphicsSystem::Get()->BeginRender();
	BatchRenderer::Get()->Render();
	GraphicsSystem::Get()->EndRender();

	// Initialize Remaining Systems
	DebugUI::StaticInitialize(handle, true, true);
	SimpleDraw::StaticInitialize(1024 * 1024);

	//Initialize the input systems
	InputSystem::StaticInitialize(handle);

	//Initialize the starting state
	mCurrentState = mAppStates.begin()->second.get();
	mCurrentState->Initialize();

	if (startTimer > TimeUtil::GetTime())
		Sleep(2000);

	if(mAppConfig.maximizeWindow)
		ShowWindow(mWindow.GetWindowHandle(), SW_MAXIMIZE);

	LOG("App -- Entering main loop ...");
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

		if (mAppConfig.escapeToQuit && inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			Quit();
			continue;
		}

		constexpr float deltaTime = 1.0f / 60.0f;
		//float deltaTime = Math::Min( TimeUtil::GetDeltaTime(), 1.0f / 60.0f);
		//float deltaTime = TimeUtil::GetDeltaTime();
		//LOG("dt = %.5f",deltaTime);
		mCurrentState->Update(deltaTime);

		auto graphicsSystem = GraphicsSystem::Get();
		graphicsSystem->BeginRender();

		mCurrentState->Render();

		BatchRenderer::Get()->Render();

		DebugUI::BeginRender();
		mCurrentState->DebugUI();
		DebugUI::EndRender();

		graphicsSystem->EndRender();
	}

	mCurrentState->Terminate();

	//Terminate engine systems
	InputSystem::StaticTerminate();

	SimpleDraw::StaticTerminate();
	DebugUI::StaticTerminate();
	MeshManager::StaticTerminate();
	ModelManager::StaticTerminate();
	SpriteRenderer::StaticTerminate();
	BatchRenderer::StaticTerminate();
	TextureManager::StaticTerminate();
	GraphicsSystem::StaticTerminate();

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
	ofn.Flags = OFN_NOCHANGEDIR;
	
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
	ofn.Flags = OFN_NOCHANGEDIR;
	return GetSaveFileNameA(&ofn);
}

float App::GetTime()
{
	return Core::TimeUtil::GetTime();
}