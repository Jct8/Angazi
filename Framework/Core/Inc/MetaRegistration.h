#pragma once

#include "Meta.h"

// Primitive Type Declarations
META_TYPE_DECLARE(uint32_t)
META_TYPE_DECLARE(int)
META_TYPE_DECLARE(float)
META_TYPE_DECLARE(bool)
META_TYPE_DECLARE(std::string)
META_TYPE_DECLARE(std::filesystem::path)

namespace Angazi::Core
{
	void StaticMetaRegister();
}