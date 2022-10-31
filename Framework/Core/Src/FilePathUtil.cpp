#include "Precompiled.h"
#include "FilePathUtil.h"

using namespace Angazi::Core;

namespace
{
	static std::string assetFilePath = "../Assets/";
}

std::string& FilePath::GetAssetFilePath()
{
	return assetFilePath;
}

