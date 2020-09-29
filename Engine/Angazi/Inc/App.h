#pragma once

#include "AppState.h"

namespace Angazi
{
	struct AppConfig
	{
		std::string appName = "Angazi";
		uint32_t windowWidth = 1280;
		uint32_t windowHeight = 720;
		bool escapeToQuit = true;
		bool maximizeWindow = false;
	};

	class App
	{
	public:
		//template<class StateType, class = std::void_t<std::is_base_of<AppState,StateType>>>
		template<class StateType>
		void AddState(std::string name)
		{
			static_assert(std::is_base_of_v<AppState, StateType>, "Cannot add type T which is not derived from AppState.");
			mAppStates.emplace(std::move(name), std::make_unique<StateType>());
		}

		void ChangeState(const std::string& name);

		void Run(AppConfig appConfig);
		void Quit() { mRunning = false; };

		// filter string has format "<Description>\0*.<extension>\0", with multiple entries separated by ;
		// "PNG Files (*.png)\0*.png;\0JPG Files (*.jpg)\0*.jpg"
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