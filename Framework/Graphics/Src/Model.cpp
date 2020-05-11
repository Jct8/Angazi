#include "Precompiled.h"
#include "Texture.h"
#include "Material.h"
#include "Model.h"
#include "MeshIO.h"
#include "SkeletonIO.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void ModelLoader::LoadSkeleton(std::filesystem::path fileName, Skeleton& skeleton)
{
	fileName.replace_extension("skeleton");
	// Homework:
	// Do loading
	// Resolve link here
	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");
	SkeletonIO::Read(file, skeleton);
	fclose(file);

	for (size_t i = 0; i < skeleton.bones.size(); ++i)
	{
		Bone* bone = skeleton.bones[i].get();
		if (bone->parentIndex == -1)
			skeleton.root = bone;
		else
			bone->parent = skeleton.bones[bone->parentIndex].get();

		for (size_t j = 0; j < bone->childIndices.size(); j++)
			bone->children.push_back(skeleton.bones[bone->childIndices[j]].get());
	}

}

void ModelLoader::LoadModel(std::filesystem::path fileName, Model& model)
{
	fileName.replace_extension("model");

	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");

	uint32_t numMeshes = 0;
	fscanf_s(file, "MeshCount: %d\n", &numMeshes);
	model.meshData.resize(numMeshes);
	for (uint32_t i = 0; i < numMeshes; i++)
	{
		fscanf_s(file, "MaterialIndex: %d\n", &model.meshData[i].materialIndex);
		MeshIO::Read(file, model.meshData[i].mesh);
	}

	uint32_t numMaterials = 0;
	fscanf_s(file, "MaterialCount: %d\n", &numMaterials);
	model.materialData.resize(numMaterials);
	for (uint32_t i = 0; i < numMaterials; i++)
	{
		char buffer[128];
		fscanf_s(file, "DiffuseMapName: %s\n", buffer, std::size(buffer));
		if (buffer != "<none>")
			model.materialData[i].diffuseMapName = buffer;
		fscanf_s(file, "MaterialAmbient: %f %f %f\n"
			, &model.materialData[i].material.ambient.x, &model.materialData[i].material.ambient.y, &model.materialData[i].material.ambient.z);
		fscanf_s(file, "MaterialDiffuse: %f %f %f\n"
			, &model.materialData[i].material.diffuse.x, &model.materialData[i].material.diffuse.y, &model.materialData[i].material.diffuse.z);
		fscanf_s(file, "MaterialSpecular: %f %f %f\n"
			, &model.materialData[i].material.specular.x, &model.materialData[i].material.specular.y, &model.materialData[i].material.specular.z);
		fscanf_s(file, "MaterialPower: %f\n", &model.materialData[i].material.power);
	}

	//For homework, save out model.materialData as well ...
	// if read <none> for texture name, there is not texture;

	fclose(file);

	for (auto& data : model.meshData)
		data.meshBuffer.Initialize(data.mesh);
	for (auto& data : model.materialData)
	{
		if (!data.diffuseMapName.empty())
		{
			data.diffuseMap = std::make_unique<Texture>();
			data.diffuseMap->Initialize(data.diffuseMapName);
		}
	}
}


void Model::Initialize(const std::filesystem::path& fileName)
{
	ASSERT(std::filesystem::exists(fileName), "File does not exist");
	ModelLoader::LoadModel(fileName, *this);
	ModelLoader::LoadSkeleton(fileName, skeleton);
}

void Model::Terminate()
{
	for (auto& data : meshData)
		data.meshBuffer.Terminate();
	for (auto& data : materialData)
		data.diffuseMap->Terminate();
}

void Model::Draw() const
{
	for (size_t i = 0; i < meshData.size(); ++i)
	{
		auto& data = meshData[i];

		materialData[data.materialIndex].diffuseMap->BindPS(0);
		data.meshBuffer.Draw();
	}
}