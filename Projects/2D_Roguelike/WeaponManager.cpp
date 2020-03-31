#include "WeaponManager.h"
#include "Ranged.h"
#include "Melee.h"

namespace
{
	std::unique_ptr<WeaponManager> sInstance = nullptr;
	const int maxsize = 100;
}

void WeaponManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "WeaponManager already initialized");
	sInstance = std::make_unique<WeaponManager>();
}

void WeaponManager::StaticTerminate()
{
	sInstance->Unload();
	sInstance.reset();
}

WeaponManager & WeaponManager::Get()
{
	ASSERT(sInstance != nullptr, "No WeaponManager created!");
	return *sInstance;
}

void WeaponManager::Load()
{
	char name[maxsize];
	char type[maxsize];
	char fileName[maxsize];

	int total = 0;

	FILE* file = nullptr;
	fopen_s(&file, "WeaponList.txt", "r");
	fscanf_s(file, "Total:%d\n", &total);

	for (int i = 0; i < total; i++)
	{
		fscanf_s(file, "%s %s %s\n", name, maxsize, type, maxsize, &fileName, maxsize);
		std::string strName(name);
		std::string strType(type);
		std::string strFileName(fileName);
		if (strType == "Melee")
			mWeapons[strName] = std::make_unique<MeleeWeapon>();
		else if (strType == "Ranged")
			mWeapons[strName] = std::make_unique<RangedWeapon>();
		mWeapons[strName]->Load(fileName);
	}

	fclose(file);
}

void WeaponManager::Unload()
{
	std::map<std::string, std::unique_ptr<Weapon>>::iterator iter = mWeapons.begin();
	for ( ; iter != mWeapons.end() ; ++iter)
	{
		iter->second->Unload();
	}
	mWeapons.clear();
}

std::unique_ptr<Weapon>& WeaponManager::GetWeapon(std::string name)
{
	return mWeapons[name];
}
