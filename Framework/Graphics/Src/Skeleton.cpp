#include "Precompiled.h"
#include "Skeleton.h"

#include "SimpleDraw.h"

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	void DrawBone(Bone* bone, const std::vector<Math::Matrix4>& boneMatrices)
	{
		Math::Vector3 bonePositiion = GetTranslation(boneMatrices[bone->index]);
		for (size_t i = 0; i < bone->children.size(); i++)
		{
			Math::Vector3 childPositiion = GetTranslation(boneMatrices[bone->children[i]->index]);
			//SimpleDraw::AddLine(bonePositiion, childPositiion, Colors::AliceBlue);
			auto direction = childPositiion - bonePositiion;
			SimpleDraw::AddCone(bonePositiion, Normalize(direction), Magnitude(direction), 1.0f, Colors::AliceBlue);
			DrawBone(bone->children[i], boneMatrices);
		}
	}
}

void Angazi::Graphics::DrawSkeleton(const Skeleton& skeleton, const std::vector<Math::Matrix4>& boneMatrices)
{
	// Homework
	// Use skeleton so you know what the parent child order is
	// But, use boneMatrices (Which is the multiplied out matrices) to get the position
	// Draw line to connect the bones
	DrawBone(skeleton.root,boneMatrices);
}

void Angazi::Graphics::ComputeBoneMatrices(Bone* bone, std::vector<Math::Matrix4>& boneMatrices)
{
	if (bone->parent)
		boneMatrices[bone->index] = bone->toParentTransform * boneMatrices[bone->parent->index];
	else
		boneMatrices[bone->index] = Math::Matrix4::Identity;

	for (size_t i = 0; i < bone->children.size(); i++)
		ComputeBoneMatrices(bone->children[i], boneMatrices);
}