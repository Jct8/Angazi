#pragma once

#include "Common.h"
#include "Colors.h"

namespace Angazi::Graphics
{
	constexpr uint32_t VE_Position =	 0x1 << 0;// 0000 0001
	constexpr uint32_t VE_Normal =		 0x1 << 1;// 0000 0010
	constexpr uint32_t VE_Tangent =		 0x1 << 2;// 0000 0100
	constexpr uint32_t VE_Color =		 0x1 << 3;// 0000 1000
	constexpr uint32_t VE_TexCoord	=	 0x1 << 4;// 0001 0000
	constexpr uint32_t VE_BlendIndex =	 0x1 << 4;// 0010 0000
	constexpr uint32_t VE_BlendWeight =	 0x1 << 4;// 0100 0000

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
		Math::Vector2 texcoord;
	};

	struct VertexPN
	{
		VERTEX_FORMAT(VE_Position | VE_Normal);
		Math::Vector3 position;
		Math::Vector3 normal;
	};

	struct Vertex
	{
		VERTEX_FORMAT(VE_Position | VE_Normal | VE_Tangent | VE_TexCoord);
		Math::Vector3 position;
		Math::Vector3 normal;
		Math::Vector3 tangent;
		Math::Vector2 texcoord;
	};

	struct BoneVertex
	{
		VERTEX_FORMAT(VE_Position | VE_Normal | VE_Tangent | VE_TexCoord | VE_BlendIndex | VE_BlendWeight);
		Math::Vector3 position;
		Math::Vector3 normal;
		Math::Vector3 tangent;
		Math::Vector2 texcoord;
		int boneIndices[4] = { 0};
		float boneWeights[4] = { 0.0f };
	};

}//namespace Angazi::Graphics