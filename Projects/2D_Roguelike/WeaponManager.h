#pragma once
#include "Weapon.h"
#include <XEngine.h>

class WeaponManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static WeaponManager& Get();

	void Load();
	void Unload();

	std::unique_ptr<Weapon>& GetWeapon(std::string name);

private:
	std::map<std::string, std::unique_ptr<Weapon>> mWeapons;

};
