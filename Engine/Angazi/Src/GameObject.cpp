#include "Precompiled.h"
#include "GameObject.h"

#include "Component.h"
using namespace Angazi;

META_CLASS_BEGIN(GameObject)
	META_NO_FIELD
	META_NO_METHOD
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

void Angazi::GameObject::RenderShadows()
{
	for (auto& component : mComponents)
		component->RenderShadow();
}

Component* GameObject::AddComponent(const Core::Meta::MetaClass* metaClass)
{
	for (auto& component : mComponents)
		if (component->GetMetaClass() == metaClass) return component.get();

	Component* newComponent = static_cast<Component*>(metaClass->Create());
	newComponent->mOwner = this;
	mComponents.emplace_back(std::unique_ptr<Component>(newComponent));
	return newComponent;
}

void GameObject::SaveTemplate() const
{
	FILE* file = nullptr;
	fopen_s(&file, mFilePath.u8string().c_str(), "wb");

	using namespace rapidjson;
	char readBuffer[65536];
	rapidjson::FileWriteStream os(file, readBuffer, sizeof(readBuffer));

	rapidjson::Document document;
	document.SetObject();

	Value gameObject(kObjectType);
	Value componentsObj(kObjectType);
	SaveGameObject(componentsObj, document);

	gameObject.AddMember("Components", componentsObj, document.GetAllocator());
	document.AddMember("GameObject", gameObject, document.GetAllocator());

	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
	document.Accept(writer);
	fclose(file);
}

void Angazi::GameObject::SaveGameObject(rapidjson::Value& componentValue, rapidjson::Document& document) const
{
	using namespace rapidjson;
	for (auto& component : mComponents)
	{
		Value componentName(kObjectType);
		Value componentsFields(kObjectType);

		auto metaClass = component->GetMetaClass();
		metaClass->Serialize(component.get(), componentsFields, document);

		componentName.SetString(component->GetMetaClass()->GetName(), document.GetAllocator());
		componentValue.AddMember(componentName, componentsFields, document.GetAllocator());
	}
}

void GameObject::Deserialize(rapidjson::GenericObject<false,rapidjson::Value>& jsonObject)
{
	if (jsonObject.HasMember("Components") && jsonObject["Components"].IsObject())
	{
		auto components = jsonObject["Components"].GetObjectW();
		for (auto& component : components)
		{
			auto metaClass = Core::Meta::FindMetaClass(component.name.GetString());
			auto newComponent = AddComponent(metaClass);
			metaClass->Deserialize(newComponent, component.value);
		}
	}
}
