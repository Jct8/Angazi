#include "Precompiled.h"
#include "GameWorld.h"

using namespace Angazi;

void GameWorld::Initialize(size_t capacity)
{
	ASSERT(!mInitialized, "GameWorld -- World already initialized.");

	for (auto& service : mServices)
		service->Initialize();

	mGameObjectAllocator = std::make_unique<GameObjectAllocator>(capacity);
	mGameObjectHandlePool = std::make_unique<GameObjectHandlePool>(capacity);

	mInitialized = true;
}

void GameWorld::Terminate()
{
	if (!mInitialized)
		return;

	ASSERT(!mUpdating, "GameWorld -- Cannot terminate during an update.");

	// Destroy all active objects
	mUpdating = true;
	std::for_each(mUpdateList.begin(), mUpdateList.end(), [this](auto gameObject)
	{
		Destroy(gameObject->GetHandle());
	});
	mUpdating = false;
	mUpdateList.clear();

	// Now destroy everything
	ProcessDestroyList();

	mGameObjectHandlePool.reset();
	mGameObjectAllocator.reset();

	for (auto& service : mServices)
		service->Terminate();

	mInitialized = false;
}

GameObjectHandle GameWorld::Create(const std::filesystem::path & templateFileName, std::string name)
{
	auto gameObject = GameObjectFactory::Create(*mGameObjectAllocator, templateFileName);
	if (gameObject == nullptr)
	{
		LOG("GameWorld -- Failed to create game object from template %s",templateFileName.u8string().c_str());
		return GameObjectHandle();
	}
	// Register with the handle pool
	auto handle = mGameObjectHandlePool->Register(gameObject);

	// Initialize the game object
	gameObject->mWorld = this;
	gameObject->mName = std::move(name);
	gameObject->mHandle = handle;
	gameObject->Initialize();

	// Add game object to the update list
	mUpdateList.push_back(gameObject);
	return handle;
}

GameObjectHandle GameWorld::Find(const std::string & name)
{
	auto iter = std::find_if(mUpdateList.begin(), mUpdateList.end(), [&name](auto gameObject)
	{
		return gameObject->GetName() == name;
	});
	if (iter != mUpdateList.end())
		return (*iter)->GetHandle();

	return GameObjectHandle();
}

void GameWorld::Destroy(GameObjectHandle handle)
{
	// If handle is invalid, nothing to do so just exit
	if (!handle.IsValid())
		return;

	// Cache the pointer and unregister with the handle pool.
	// No one can touch this game object anymore
	GameObject* gameObject = handle.Get();
	mGameObjectHandlePool->Unregister(handle);

	// Check if we can destroy it now
	if (mUpdating)
		mDestroyList.push_back(gameObject);
	else
		DestroyInternal(gameObject);



}

void GameWorld::Update(float deltaTime)
{
	ASSERT(!mUpdating, "GameWorld -- Already updaing the world!");

	// Lock the update list
	mUpdating = true;

	for (auto& service : mServices)
		service->Update(deltaTime);

	// Re-compute size in case new objects are added to the update
	// list during iteration.
	for (size_t i = 0; i < mUpdateList.size(); ++i)
	{
		GameObject* gameObject = mUpdateList[i];
		if (gameObject->GetHandle().IsValid())
			gameObject->Update(deltaTime);
	}

	// Unlock the update list
	mUpdating = false;

	// Now we can safely destroy objects
	ProcessDestroyList();
}

void GameWorld::Render()
{
	for (auto& service : mServices)
		service->Render();
	for (auto gameObject : mUpdateList)
		gameObject->Render();
}

void GameWorld::DebugUI()
{
	for (auto& service : mServices)
		service->DebugUI();
	for (auto gameObject : mUpdateList)
		gameObject->DebugUI();
}

void Angazi::GameWorld::DestroyInternal(GameObject * gameObject)
{
	ASSERT(!mUpdating, "GameWorld -- Cannot destroy game objects during the update.");

	// If pointer is invalid, nothing to do so just exit
	if (gameObject == nullptr)
		return;

	// First remove it from the update list
	auto iter = std::find(mUpdateList.begin(), mUpdateList.end(), gameObject);
	if (iter != mUpdateList.end())
		mUpdateList.erase(iter);

	// Terminate the game object
	gameObject->Terminate();

	// Next destroy the game object
	GameObjectFactory::Destory(*mGameObjectAllocator, gameObject);
}

void Angazi::GameWorld::ProcessDestroyList()
{
	for (auto gameObject : mDestroyList)
		DestroyInternal(gameObject);
	mDestroyList.clear();
}
