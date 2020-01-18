#pragma once

#include "Common.h"
#include "Colors.h"

namespace Angazi::Graphics
{
	constexpr uint32_t VE_Position = 0x1 << 0;// 0000 0001
	constexpr uint32_t VE_Normal =	 0x1 << 1;// 0000 0010
	constexpr uint32_t VE_Tangent =	 0x1 << 2;// 0000 0100
	constexpr uint32_t VE_Color =	 0x1 << 3;// 0000 1000
	constexpr uint32_t VE_TexCoord = 0x1 << 4;// 0001 0000

#define VERTEX_FORMAT(fmt)\
	static constexpr uint32_t Format = fmt;

	struct VertexPC
	{
		VERTEX_FORMAT(VE_Position | VE_Color);
		Math::Vector3 position;
		Color color;
	};

	struct VertexPX
	{
		VERTEX_FORMAT(VE_Position | VE_TexCoord);
		Math::Vector3 position;
		float u, v;
	};

	struct VertexPN
	{
		VERTEX_FORMAT(VE_Position | VE_Normal);
		Math::Vector3 position;
		Math::Vector3 normal;
	};

}//namespace Angazi::Graphics