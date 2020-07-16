#include "Precompiled.h"
#include "SkeletonIO.h"

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	void WriteMatrix(FILE* file, const Math::Matrix4& m)
	{
		for (size_t i = 0; i < std::size(m.v); i++)
			fprintf_s(file, "%f ", m.v[i]);
	}

	void ReadMatrix(FILE* file, Math::Matrix4& m)
	{
		for (size_t i = 0; i < std::size(m.v); i++)
			fscanf_s(file, "%f ", &m.v[i]);
	}
}

void SkeletonIO::Write(FILE * file, const Skeleton & skeleton)
{
	uint32_t numBones = static_cast<uint32_t>(skeleton.bones.size());
	fprintf_s(file, "BoneCount:%d\n", numBones);
	for (uint32_t i = 0; i < numBones; i++)
	{
		fprintf_s(file, "Name:%s i:%d p:%d "
			, skeleton.bones[i]->name.c_str()
			, skeleton.bones[i]->index
			, skeleton.bones[i]->parent ? skeleton.bones[i]->parent->index : -1);
		fprintf_s(file, "ToParentTransform:");
		WriteMatrix(file, skeleton.bones[i]->toParentTransform);
		fprintf_s(file, "OffetTransform:");
		WriteMatrix(file, skeleton.bones[i]->offsetTransform);
		uint32_t numChildren = static_cast<uint32_t>(skeleton.bones[i]->children.size());
		fprintf_s(file, "\nChildrenCount:%d, ", numChildren);
		for (uint32_t j = 0; j < numChildren; j++)
			fprintf_s(file, "%d ", skeleton.bones[i]->children[j]->index);
		fprintf_s(file, "\n");
	}
}

void SkeletonIO::Read(FILE * file, Skeleton & skeleton)
{
	uint32_t numBones;
	fscanf_s(file, "BoneCount:%d\n", &numBones);
	for (uint32_t i = 0; i < numBones; i++)
	{
		skeleton.bones.emplace_back(std::make_unique<Bone>());
		char buffer[128];
		fscanf_s(file, "Name:%s", buffer, std::size(buffer));
		skeleton.bones[i]->name = buffer;
		fscanf_s(file, " i:%d p:%d "
			, &skeleton.bones[i]->index
			, &skeleton.bones[i]->parentIndex);

		fscanf_s(file, "ToParentTransform:");
		ReadMatrix(file, skeleton.bones[i]->toParentTransform);
		fscanf_s(file, "OffetTransform:");
		ReadMatrix(file, skeleton.bones[i]->offsetTransform);

		uint32_t numChildren;
		fscanf_s(file, "\nChildrenCount:%d, ", &numChildren);
		skeleton.bones[i]->childIndices.resize(numChildren);
		for (uint32_t j = 0; j < numChildren; j++)
			fscanf_s(file, "%d ", &skeleton.bones[i]->childIndices[j]);
		fscanf_s(file, "\n");
	}
}