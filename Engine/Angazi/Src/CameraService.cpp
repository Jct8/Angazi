#include "Precompiled.h"
#include "CameraService.h"

using namespace Angazi;

META_DERIVED_BEGIN(CameraService, Service)
	META_NO_FIELD
META_CLASS_END

void CameraService::Initialize()
{
	mActiveCamera = AddCamera("Default");
}

Graphics::Camera * Angazi::CameraService::AddCamera(const char * name)
{
	return &mCameraMap[name];
}

Graphics::Camera * Angazi::CameraService::FindCamera(const char * name)
{
	auto iter = mCameraMap.find(name);
	return (iter == mCameraMap.end() ? nullptr : &(iter->second));
}

void Angazi::CameraService::SetActiveCamera(const char * name)
{
	auto camera = FindCamera(name);
	if (camera)
		mActiveCamera = camera;
}

Graphics::Camera & Angazi::CameraService::GetActiveCamera()
{
	ASSERT(mActiveCamera, "CameraService -- No active camera!");
	return *mActiveCamera;
}

const Graphics::Camera & Angazi::CameraService::GetActiveCamera() const
{
	ASSERT(mActiveCamera, "CameraService -- No active camera!");
	return *mActiveCamera;
}
