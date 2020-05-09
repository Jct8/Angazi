#include "Precompiled.h"
#include "Skeleton.h"

#include "SimpleDraw.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void Angazi::Graphics::DrawSkeleton(const Skeleton& skeleton, const std::vector<Math::Matrix4>& boneMatrices)
{
	// Homework
	// Use skeleton so you know what the parent child order is
	// But, use boneMatrices (Which is the multiplied out matrices) to get the position
	// Draw line to connect the bones

	Math::Matrix4 mat;
	// Translation Matrix by dx, dy , dz
	// 1, 0 , 0, 0
	// 0, 1 , 0, 0
	// 0, 0 , 1, 0
	// x, y , z, 0
	//Math::Vector3 pos = GetTranslation(mat);
}