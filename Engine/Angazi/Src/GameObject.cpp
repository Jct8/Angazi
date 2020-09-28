#include "Precompiled.h"
#include "GameObject.h"

#include "Component.h"
using namespace Angazi;

META_CLASS_BEGIN(GameObject)
	META_NO_FIELD
META_CLASS_END

void GameObject::Initialize()
{
	ASSERT(!mInitialized, " GameObject -- Game object already initialized.");
	for (auto& component : mComponents)
		component->Initialize();
	mInitialized = true;
}
void GameObject::Terminate()
{
	for (auto& component : mComponents)
		component->Terminate();
}

void GameObject::Update(float deltaTime)
{
	for (auto& component : mComponents)
		component->Update(deltaTime);
}
void GameObject::Render()
{
	for (auto& component : mComponents)
		component->Render();
}
void GameObject::DebugUI()
{
	for (auto& component : mComponents)
		component->DebugUI();
}

Component* GameObject::AddComponent(const Core::Meta::MetaClass* metaClass)
{
	for (auto& component : mComponents)
		if (component->GetMetaClass() == metaClass) return nullptr;

	Component* newComponent = static_cast<Component*>(metaClass->Create());
	newComponent->mOwner = this;
	mComponents.emplace_back(std::unique_ptr<Component>(newComponent));
	return newComponent;
}

void GameObject::SaveGameObject() const
{
	FILE* file = nullptr;
	fopen_s(&file, mFilePath.u8string().c_str(), "wb");

	using namespace rapidjson;
	char readBuffer[65536];
	rapidjson::FileWriteStream os(file, readBuffer, sizeof(readBuffer));

	rapidjson::Document document;
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	document.SetObject();

	Value gameObject(kObjectType);

	Value componentsObj(kObjectType);
	for (auto& component : mComponents)
	{
		Value componentName(kObjectType);
		Value componentsFields(kObjectType);

		auto metaClass = component->GetMetaClass();
		metaClass->Serialize(component.get(), componentsFields, document);

		componentName.SetString(component->GetMetaClass()->GetName(),allocator);
		componentsObj.AddMember(componentName, componentsFields, allocator);
	}

	gameObject.AddMember("Components", componentsObj, allocator);
	document.AddMember("GameObject", gameObject, allocator);

	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
	document.Accept(writer);
	fclose(file);
}
