#include "Precompiled.h"
#include "Skeleton.h"

#include "SimpleDraw.h"

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	int j = 1;
	void DrawBone(Bone* bone, const std::vector<Math::Matrix4>& boneMatrices, float scale)
	{
		Math::Vector3 bonePosition = GetTranslation(boneMatrices[bone->index]);
		for (size_t i = 0; i < bone->children.size(); i++)
		{
			Math::Vector3 childPosition = GetTranslation(boneMatrices[bone->children[i]->index]);
			auto direction = childPosition - bonePosition;
			//SimpleDraw::AddLine(bonePosition, childPosition, Colors::AliceBlue);
			//SimpleDraw::AddCone(bonePosition, Normalize(direction), Magnitude(direction), 1.0f, Colors::AliceBlue);
			SimpleDraw::AddBone(bonePosition, direction, Colors::AliceBlue, scale *0.01f, true);
			//SimpleDraw::AddTransform(boneMatrices[bone->index]);
			DrawBone(bone->children[i], boneMatrices, scale);
		}
		if (j == 14)
		{
			//SimpleDraw::AddTransform(boneMatrices[bone->index]);
			//Math::Vector3 childPosition = GetTranslation(boneMatrices[bone->children[0]->index]);
			//auto direction =  bonePosition - childPosition;
			//SimpleDraw::AddBone(bonePosition, direction, Colors::AliceBlue, scale *0.01f, true);
			//SimpleDraw::AddTransform(boneMatrices[bone->children[0]->index]);
		}
		j++;
	}
}

void Angazi::Graphics::DrawSkeleton(const Skeleton& skeleton, const std::vector<Math::Matrix4>& boneMatrices, float scale)
{
	j = 0;
	DrawBone(skeleton.root, boneMatrices, scale);
}

void Angazi::Graphics::UpdateBoneMatrices(Bone* bone, std::vector<Math::Matrix4>& boneMatrices, bool applyOffset, const AnimationClip& clip, float time)
{
	Math::Matrix4 transform = bone->toParentTransform;
	if (!clip.boneAnimations.empty())
		clip.GetTransform(time, bone->index, transform);

	if (bone->parent)
		boneMatrices[bone->index] = transform * boneMatrices[bone->parent->index];
	else
		boneMatrices[bone->index] = transform;

	for (size_t i = 0; i < bone->children.size(); i++)
		UpdateBoneMatrices(bone->children[i], boneMatrices, applyOffset, clip, time);

	if (applyOffset)
		boneMatrices[bone->index] = Math::Transpose(bone->offsetTransform * boneMatrices[bone->index] );
}

void Angazi::Graphics::UpdateBoneMatrices(Bone* bone, std::vector<Math::Matrix4>& boneMatrices,
	const AnimationClip& clipFrom, const AnimationClip& clipTo, float blendWeight, bool applyOffset, float time)
{
	Math::Matrix4 transform = bone->toParentTransform;
	std::tuple<Math::Vector3, Math::Quaternion, Math::Vector3> tupleFrom;
	std::tuple<Math::Vector3, Math::Quaternion, Math::Vector3> tupleTo;
	if (clipFrom.GetTransformTuple(time, bone->index, tupleFrom) && (clipTo.GetTransformTuple(time, bone->index, tupleTo)))
	{
		Math::Vector3 position = Lerp(std::get<0>(tupleFrom), std::get<0>(tupleTo), blendWeight);
		Math::Quaternion rotation = Slerp(std::get<1>(tupleFrom), std::get<1>(tupleTo), blendWeight);
		Math::Vector3 scale = Lerp(std::get<2>(tupleFrom), std::get<2>(tupleTo), blendWeight);
		auto matTrans = Math::Matrix4::Translation({ position });
		auto matRot = Math::Matrix4::RotationQuaternion(rotation);
		auto matScale = Math::Matrix4::Scaling(scale);
		transform = matScale * matRot * matTrans;
	}

	if (bone->parent)
		boneMatrices[bone->index] = transform * boneMatrices[bone->parent->index];
	else
		boneMatrices[bone->index] = transform;

	for (size_t i = 0; i < bone->children.size(); i++)
		UpdateBoneMatrices(bone->children[i], boneMatrices, clipFrom, clipTo, blendWeight, applyOffset, time);

	if (applyOffset)
		boneMatrices[bone->index] = Math::Transpose(bone->offsetTransform * boneMatrices[bone->index]);
}

//Tips for getting the bone matrices :
//---------------------------------- -
//
//			R
//			|
//			V
//		   [U]
//       [U]  [U]
//    [U]        [U]
//
//GetTransform returns
//bone->toParentTransform if we are using the bind pose(default from the skeleton)
//clip.boneAnim[bone->index] if we are using a animation pose(at time t)
//
//void UpdateBoneRecursive(std::vector<Matrix>& boneMatrices, const Bone* bone)
//{
//	if (bone->parent)
//		boneMatrices[bone->index] = GetTransform(bone) * boneMatrices[bone->parent->index]
//	else
//		boneMatrices[bone->index] = GetTransform(bone)
//
//		for (auto& child : bone->children)
//			UpdateBoneRecursive(boneMatrices, child)
//}
//
//
//std::vector<Matrix> boneMatrices;
//UpdateBoneRecursive(boneMatrices, skeleton.root)
//

