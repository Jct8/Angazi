#pragma once

#include "AppState.h"

namespace Angazi
{
	enum class GraphicsAPI
	{
		DirectX,
		OpenGL
	};

	struct AppConfig
	{
		std::string appName = "Angazi";
		uint32_t windowWidth = 1280;
		uint32_t windowHeight = 720;
		GraphicsAPI api = GraphicsAPI::DirectX;
	};

	class App
	{
	public:
		template<class StateType, class = std::void_t<std::is_base_of<AppState,StateType>>>
		void AddState(std::string name)
		{
			mAppStates.emplace(std::move(name), std::make_unique<StateType>());
		}

		void ChangeState(const std::string& name);

		void Run(AppConfig appConfig);
		void Quit() { mRunning = false; };

		bool OpenFileDialog(char fileName[MAX_PATH], const char* title, const char* filter);
		bool SaveFileDialog(char fileName[MAX_PATH], const char* title, const char* filter);
		float GetTime();

	private:
		AppConfig mAppConfig;
		Core::Window mWindow;
		bool mRunning = false;

		std::map<std::string, std::unique_ptr<AppState>> mAppStates;
		AppState *mCurrentState = nullptr;
		AppState *mNextState = nullptr;

	};

}