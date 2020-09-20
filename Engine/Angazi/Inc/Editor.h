#pragma once

#include "EditorLogView.h"

namespace Angazi
{
	class GameObject;
	class GameWorld;
	class Service;

	class Editor final
	{
	public:
		Editor(GameWorld& world);
		~Editor();

		void Show();

		void ShowWorldView();
		void ShowInspectorView();
		void ShowLogView();

		bool ShouldQuit() const;

	public:
		void ShowMenuBar();
		void ShowFileMenu();
		void ShowEditMenu();

		void ShowMainWindowWithDockSpace();

		void New();
		void Open();
		void Save();
		void SaveAs();
		void Exit();

		GameWorld& mWorld;

		Service* mSelectedService = nullptr;
		GameObject* mSelectedGameObject = nullptr;

		EditorLogView mEditorLogView;
		Core::EventHandlerId mDebugLogHandlerId = 0;

		bool mRequestQuit = false;
	};

}