#include "Precompiled.h"
#include "Model.h"

#include "AnimationIO.h"
#include "Material.h"
#include "MeshIO.h"
#include "SkeletonIO.h"
#include "StandardEffect.h"
#include "TextureManager.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void ModelLoader::LoadSkeleton(std::filesystem::path fileName, Skeleton& skeleton)
{
	fileName.replace_extension("skeleton");
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

void ModelLoader::LoadAnimationSet(std::filesystem::path fileName, AnimationSet & animationSet)
{
	fileName.replace_extension("animset");

	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");
	uint32_t clipCount;
	fscanf_s(file, "ClipCount: %d\n", &clipCount);
	
	animationSet.clips.resize(clipCount);
	for (uint32_t i = 0; i < clipCount; i++)
	{
		animationSet.clips[i] = std::make_unique<AnimationClip>();
		AnimationIO::Read(file, *animationSet.clips[i]);
	}
	fclose(file);
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
		// if read <none> for texture name, there is not texture;
		char buffer[128];
		fscanf_s(file, "DiffuseMapName: %s\n", buffer, std::size(buffer));
		if (std::string(buffer) != "<none>")
			model.materialData[i].diffuseMapName = buffer;
		fscanf_s(file, "NormalMapName: %s\n", buffer, std::size(buffer));
		if (std::string(buffer) != "<none>")
			model.materialData[i].normalMapName = buffer;

		fscanf_s(file, "MaterialAmbient: %f %f %f\n"
			, &model.materialData[i].material.ambient.x, &model.materialData[i].material.ambient.y, &model.materialData[i].material.ambient.z);
		fscanf_s(file, "MaterialDiffuse: %f %f %f\n"
			, &model.materialData[i].material.diffuse.x, &model.materialData[i].material.diffuse.y, &model.materialData[i].material.diffuse.z);
		fscanf_s(file, "MaterialSpecular: %f %f %f\n"
			, &model.materialData[i].material.specular.x, &model.materialData[i].material.specular.y, &model.materialData[i].material.specular.z);
		fscanf_s(file, "MaterialPower: %f\n", &model.materialData[i].material.power);
	}



	fclose(file);

	for (auto& data : model.meshData)
		data.meshBuffer.Initialize(data.mesh);
	for (auto& data : model.materialData)
	{
		if (!data.diffuseMapName.empty())
		{
			data.diffuseMap = std::make_unique<Texture>();
			data.diffuseMap->Initialize(fileName.replace_filename(data.diffuseMapName),true);
		}
		if (!data.normalMapName.empty())
		{
			data.normalMap = std::make_unique<Texture>();
			data.normalMap->Initialize(fileName.replace_filename(data.normalMapName));
		}
	}
}

void Model::Initialize(const std::filesystem::path& fileName)
{
	ASSERT(std::filesystem::exists(fileName), "File does not exist");
	ModelLoader::LoadModel(fileName, *this);
	ModelLoader::LoadSkeleton(fileName, skeleton);
	ModelLoader::LoadAnimationSet(fileName,animationSet);
	mBlendState.Initialize(BlendState::Mode::Additive);
	mDepthStencilState.Initialize(true, true);

}

void Model::Terminate()
{
	mDepthStencilState.Terminate();
	mBlendState.Terminate();
	for (auto& data : meshData)
		data.meshBuffer.Terminate();
	for (auto& data : materialData)
		if(data.diffuseMap)
			data.diffuseMap->Terminate();
	for (auto& data : materialData)
		if (data.normalMap)
			data.normalMap->Terminate();
}

void Model::Draw(Effect *effect) const
{
	for (size_t i = 0; i < meshData.size(); ++i)
	{
		auto& data = meshData[i];
		//materialData[data.materialIndex].diffuseMap->BindPS(0);
		//materialData[data.materialIndex].normalMap->BindPS(3);
		if (effect->GetEffectType() == Effect::EffectType::StandardType)
		{
			StandardEffect* standardEffect = static_cast<StandardEffect*>(effect);
			if(materialData[data.materialIndex].diffuseMap)
				standardEffect->SetDiffuseTexture(materialData[data.materialIndex].diffuseMap.get());
			if(materialData[data.materialIndex].normalMap)
				standardEffect->SetNormalTexture(materialData[data.materialIndex].normalMap.get());
			//standardEffect->SetMaterial(materialData[data.materialIndex].material);
		}
		//if (materialData[data.materialIndex].diffuseMapName == "Sponza_diffuse_14.png"
		//	|| materialData[data.materialIndex].diffuseMapName == "Sponza_diffuse_9.png"
		//	|| materialData[data.materialIndex].diffuseMapName == "Sponza_diffuse_11.png")
		//{
		//	mDepthStencilState.Set();
		//	mBlendState.Bind();
		//}
		data.meshBuffer.Draw();
		mBlendState.ClearState();
		mDepthStencilState.Clear();
	}
}