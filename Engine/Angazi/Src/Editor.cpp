#include "Precompiled.h"
#include "Editor.h"

#include "Component.h"
#include "GameWorld.h"
#include "GameObject.h"
#include "ImGui/Inc/imgui.h"
#include "Service.h"

using namespace Angazi;

Editor::Editor(GameWorld & world)
	:mWorld(world)
{
	mDebugLogHandlerId = Core::OnDebugLog +=[this](auto str) {mEditorLogView.AddLog(str.c_str());};
}

Editor::~Editor()
{
	Core::OnDebugLog -= mDebugLogHandlerId;
}

void Editor::Show()
{
	ShowMenuBar();
	ShowMainWindowWithDockSpace();

	ShowWorldView();
	ShowInspectorView();
	ShowLogView();
}

void Editor::ShowWorldView()
{
	ImGui::Begin("World");
	if (ImGui::TreeNode("Services"))
	{
		for (auto& service : mWorld.mServices)
		{
			if (ImGui::Selectable(service->GetMetaClass()->GetName(),service.get() == mSelectedService))
			{
				mSelectedService = service.get();
				mSelectedGameObject = nullptr;
			}
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Game Objects"))
	{
		for (auto gameObject : mWorld.mUpdateList)
		{
			if (ImGui::Selectable(gameObject->GetName().c_str(), gameObject == mSelectedGameObject))
			{
				mSelectedGameObject = gameObject;
				mSelectedService = nullptr;
			}
		}
		ImGui::TreePop();
	}
	ImGui::End();
}

void Editor::ShowInspectorView()
{
	ImGui::Begin("Inspector");
	if (mSelectedService)
	{
		mSelectedService->DebugUI();
	}
	else if (mSelectedGameObject)
	{
		for (auto& component : mSelectedGameObject->mComponents)
		{
			auto metaClass = component->GetMetaClass();
			if (ImGui::CollapsingHeader(metaClass->GetName(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				for (size_t i = 0; i < metaClass->GetFieldCount(); i++)
				{
					auto metaField = metaClass->GetField(i);
					if (metaField->GetMetaType() == Core::Meta::GetMetaType<Math::Vector3>())
					{
						auto data = (float*) metaField->GetFieldInstance(component.get());
						ImGui::DragFloat3(metaField->GetName(), data);
					}
				}
			}
		}
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
	if (ImGui::MenuItem("New", nullptr, false, false))
		New();
	if (ImGui::MenuItem("Open", "Ctrl+O", false, false))
		Open();
	if (ImGui::MenuItem("Save", "Ctrl+S", false, false))
		Save();
	if (ImGui::MenuItem("Save As..", "Ctrl+Shit+S", false, false))
		SaveAs();

	ImGui::Separator();
}

void Editor::ShowEditMenu()
{
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

void Editor::New()
{
}

void Editor::Open()
{
}

void Editor::Save()
{
}

void Editor::SaveAs()
{
}
