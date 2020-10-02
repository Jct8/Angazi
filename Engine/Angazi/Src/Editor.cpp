#include "Precompiled.h"
#include "Editor.h"

#include "Component.h"
#include "GameWorld.h"
#include "GameObject.h"
#include "ImGui/Inc/imgui.h"
#include "Service.h"
#include "Angazi.h"

using namespace Angazi;
using namespace Angazi::Input;

Editor::Editor(GameWorld& world)
	:mWorld(world)
{
	mDebugLogHandlerId = Core::OnDebugLog += [this](auto str) {mEditorLogView.AddLog(str.c_str()); };
}

Editor::~Editor()
{
	Core::OnDebugLog -= mDebugLogHandlerId;
}

void Editor::Show()
{
	ShowMenuBar();
	ShowModals();
	ShowMainWindowWithDockSpace();
	
	ImGui::Begin("Style");
	ImGui::ShowStyleEditor();
	ImGui::End();

	ShowStatsMenu();
	ShowWorldView();
	ShowInspectorView();
	ShowLogView();
}

void Editor::ShowWorldView()
{
	bool gameobjectflag = false;
	ImGui::Begin("Scene Hierarchy");

	ImGui::SetNextItemOpen(true);
	if (ImGui::TreeNode("Services"))
	{
		for (auto& service : mWorld.mServices)
		{
			if (ImGui::Selectable(service->GetMetaClass()->GetName(), service.get() == mSelectedService))
			{
				mSelectedService = service.get();
				mSelectedGameObject = nullptr;
			}
		}
		ImGui::TreePop();
	}
	ImGui::SetNextItemOpen(true);
	if (ImGui::TreeNode("Game Objects"))
	{
		for (auto gameObject : mWorld.mUpdateList)
		{
			ImGui::PushID(gameObject);

			if (!gameObject->mEnabled)
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.46f, 0.46f, 0.46f, 1.00f));
			if (ImGui::Selectable(gameObject->GetName().c_str(), gameObject == mSelectedGameObject))
			{
				mSelectedGameObject = gameObject;
				mSelectedService = nullptr;
			}
			if (!gameObject->mEnabled)
				ImGui::PopStyleColor(1);

			if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right))
				ImGui::OpenPopup("GameObject_options");

			if (ImGui::BeginPopup("GameObject_options") )
			{
				gameobjectflag = true;
				std::string deleteText = "Delete " +gameObject->GetName();
				if (ImGui::Selectable(deleteText.c_str()))
				{
					mWorld.Destroy(gameObject->GetHandle());
					mSelectedGameObject = nullptr;
				}
				ImGui::EndPopup();
			}
			ImGui::PopID();
		}
		ImGui::TreePop();
	}
	if (InputSystem::Get()->IsMousePressed(MouseButton::RBUTTON) && ImGui::IsWindowHovered() && !gameobjectflag)
		ImGui::OpenPopup("Add_GameObject");

	if (ImGui::BeginPopup("Add_GameObject"))
	{
		if (ImGui::Selectable("Add Empty GameObject"))
		{
			auto go = mWorld.CreateEmpty();
			mSelectedGameObject = go.Get();
		}
		ImGui::EndPopup();
	}
	ImGui::End();
}

void Editor::ShowInspectorView()
{
	ImGui::Begin("Properties");
	if (mSelectedService)
	{
		ImGui::Checkbox(mSelectedService->GetMetaClass()->GetName(), &mSelectedService->mEnabled);
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();

		mSelectedService->ShowInspectorProperties();
	}
	else if (mSelectedGameObject)
	{
		char buffer[128];
		ImGui::PushID(mSelectedGameObject);
		ImGui::Checkbox("", &mSelectedGameObject->mEnabled);
		ImGui::SameLine();
		strcpy_s(buffer, mSelectedGameObject->GetName().c_str());
		if (ImGui::InputText("##namebuffer", buffer, IM_ARRAYSIZE(buffer)))
			mSelectedGameObject->SetName(buffer);
		
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();
		for (auto& component : mSelectedGameObject->mComponents)
			component->ShowInspectorProperties();

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("Add_Component");

		if (ImGui::BeginPopup("Add_Component"))
		{
			auto metaClasses = Angazi::Core::Meta::GetAllMetaClasses();
			for (auto& [name, metaClass] : metaClasses)
			{
				if (metaClass->GetParent() == Component::StaticGetMetaClass())
				{
					if (ImGui::Selectable(name.c_str()))
					{
						auto component = mSelectedGameObject->AddComponent(metaClass);
						if(component)
							component->Initialize();
					}
				}
			}
			ImGui::EndPopup();
		}
		ImGui::PopID();
	}
	ImGui::End();
}

void Editor::ShowLogView()
{
	mEditorLogView.Draw("Log");
}

bool Editor::ShouldQuit() const
{
	return mRequestQuit;
}

void Editor::ShowMenuBar()
{
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.95f);
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ShowFileMenu();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			ShowEditMenu();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	ImGui::PopStyleVar(1);

}

void Editor::ShowFileMenu()
{
	if (ImGui::MenuItem("New Scene", "Ctrl+N", false, true))
		showMessageBoxNew = true;
	if (ImGui::MenuItem("Open Scene", "Ctrl+O", false, true))
		showMessageBoxOpen = true;
	if (ImGui::MenuItem("Save", "Ctrl+S", false, true))
		Save();
	if (ImGui::MenuItem("Save As..", "Ctrl+Shit+S", false, true))
		SaveAs();
	if (ImGui::MenuItem("Exit", nullptr, false))
		showMessageBoxExit = true;

	ImGui::Separator();
}

void Editor::ShowEditMenu()
{

}

void Editor::ShowStatsMenu()
{
	ImGui::Begin("Stats");
	ImGui::Text("FPS: %.2f", Core::TimeUtil::GetFramesPerSecond());
	ImGui::End();
}

void Editor::ShowMainWindowWithDockSpace()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	const ImGuiWindowFlags windowFlags =
		ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("MainWindow", nullptr, windowFlags);
	ImGui::DockSpace(ImGui::GetID("MyDockSpace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
	ImGui::End();

	ImGui::PopStyleVar(3);
}

void Editor::ShowModals()
{
	if (showMessageBoxExit)
	{
		ImGui::OpenPopup("Quit?");
		showMessageBoxExit = false;
	}
	if (showMessageBoxNew)
	{
		ImGui::OpenPopup("New?");
		showMessageBoxNew = false;
	}
	if (showMessageBoxOpen)
	{
		ImGui::OpenPopup("Open?");
		showMessageBoxOpen = false;
	}

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Quit?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Do you want to save\nbefore exiting?");
		ImGui::Separator();

		if (ImGui::Button("Yes", ImVec2(50, 0)))
		{
			Save();
			Angazi::MainApp().Quit();
		}
		ImGui::SetItemDefaultFocus();

		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2(50, 0)))
		{
			ImGui::CloseCurrentPopup();
			Angazi::MainApp().Quit();
		}
		ImGui::SetItemDefaultFocus();

		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(50, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("New?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Do you want to save\nbefore creating a new scene?");
		ImGui::Separator();

		if (ImGui::Button("Yes", ImVec2(50, 0)))
		{
			Save();
			New();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();

		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2(50, 0)))
		{
			ImGui::CloseCurrentPopup();
			New();
		}
		ImGui::SetItemDefaultFocus();

		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(50, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("Open?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Do you want to save\nbefore opening a new scene?");
		ImGui::Separator();

		if (ImGui::Button("Yes", ImVec2(50, 0)))
		{
			Save();
			Open();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();

		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2(50, 0)))
		{
			ImGui::CloseCurrentPopup();
			Open();
		}
		ImGui::SetItemDefaultFocus();

		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(50, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void Editor::New()
{
	mWorld.UnloadScene();
}

void Editor::Open()
{
	char filePath[MAX_PATH]{};
	const char* title = "Open Scene";
	const char* filter = "Json Files (*.json)\0*.json";
	if (MainApp().OpenFileDialog(filePath, title, filter))
	{
		std::filesystem::path openPath = filePath;
		if (std::filesystem::exists(openPath))
		{
			mWorld.UnloadScene();
			mWorld.LoadScene(openPath);
		}
	}
}

void Editor::Save()
{
	if (std::filesystem::exists(mWorld.mSceneFilePath))
		mWorld.SaveScene();
	else
		SaveAs();
}

void Editor::SaveAs()
{
	char filePath[MAX_PATH]{};
	const char* title = "Save Scene";
	const char* filter = "Json Files (*.json)\0*.json";
	if (MainApp().SaveFileDialog(filePath, title, filter))
	{
		std::filesystem::path savePath = filePath;
		if (savePath.extension().empty())
			savePath += ".json";
		mWorld.SaveScene(savePath);
	}
}

void Editor::Exit()
{
	Angazi::MainApp().Quit();
}

//ImGui::NewLine();
//auto metaClass = component->GetMetaClass();
//if (ImGui::CollapsingHeader(metaClass->GetName(), ImGuiTreeNodeFlags_DefaultOpen))
//{
//	for (size_t i = 0; i < metaClass->GetFieldCount(); i++)
//	{
//		auto metaField = metaClass->GetField(i);
//		if (metaField->GetMetaType() == Core::Meta::GetMetaType<Math::Vector3>())
//		{
//			auto data = (float*)metaField->GetFieldInstance(component.get());
//			ImGui::DragFloat3(metaField->GetName(), data);
//		}
//	}
//}