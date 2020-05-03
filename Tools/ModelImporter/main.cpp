#include <Graphics/Inc/Graphics.h>
#include <Math/Inc/EngineMath.h>

#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags
#include <cstdio>
#include "main.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Math;

struct Arguments
{
	const char *inputFileName = nullptr;
	const char *outputFileName = nullptr;
	float scale = 1.0f;
	bool animOnly = false;
};

std::optional<Arguments> ParseArgs(int argc, char* argv[])
{
	// We need at least 3 arguments.
	if (argc < 3)
		return std::nullopt;

	Arguments args;
	args.inputFileName = argv[argc - 2];
	args.outputFileName = argv[argc - 1];

	for (int i = 1; i + 2 < argc; ++i)
	{
		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
			case 's':
				args.scale = static_cast<float>(atof(argv[i + 1]));
				break;
			}
		}
	}
	return args;
}

void PrintUsage()
{
	printf
	(
		"== ModelImporter Help ==\n"
		"\n"
		"Usage:\n"
		"    ModelImporter.exe [Options] <InputFile> <OutputFile>\n"
		"\n"
		"Options:\n"
		"    -s    Scale applied to the model."
		"\n"
	);
}

void SaveModel(const Arguments& args, const Model& model)
{
	std::filesystem::path path = args.outputFileName;
	path.replace_extension("model");

	printf("Saving Model: %s...\n", path.u8string().c_str());

	FILE* file = nullptr;
	fopen_s(&file, path.u8string().c_str(), "w");

	const uint32_t numMeshes = static_cast<uint32_t>(model.meshData.size());
	fprintf_s(file, "MeshCount: %d\n", numMeshes);
	for (uint32_t i = 0; i < numMeshes; i++)
	{
		fprintf_s(file, "MaterialIndex: %d\n", model.meshData[i].materialIndex);
		MeshIO::Write(file, model.meshData[i].mesh);
	}

	const uint32_t numMaterials = static_cast<uint32_t>(model.materialData.size());
	fprintf_s(file, "MaterialCount: %d\n", numMaterials);
	for (uint32_t i = 0; i < numMaterials; i++)
	{
		if (!model.materialData[i].diffuseMapName.empty())
			fprintf_s(file, "DiffuseMapName: %s\n", model.materialData[i].diffuseMapName.c_str());
		else
			fprintf_s(file, "DiffuseMapName: <none>");
		fprintf_s(file, "MaterialAmbient: %f %f %f\n"
			, model.materialData[i].material.ambient.x, model.materialData[i].material.ambient.y, model.materialData[i].material.ambient.z);
		fprintf_s(file, "MaterialDiffuse: %f %f %f\n"
			, model.materialData[i].material.diffuse.x, model.materialData[i].material.diffuse.y, model.materialData[i].material.diffuse.z);
		fprintf_s(file, "MaterialSpecular: %f %f %f\n"
			, model.materialData[i].material.specular.x, model.materialData[i].material.specular.y, model.materialData[i].material.specular.z);
		fprintf_s(file, "MaterialPower: %f\n", model.materialData[i].material.power);
	}
	//For homework, save out model.materialData as well ...
	// If diffuseMapname is empty string , write<none>
	fclose(file);
}

inline Color Convert(const aiColor3D& c)
{
	return { c.r , c.g, c.b, 1.0f };
}

inline Vector3 Convert(const aiVector3D& v)
{
	return { v.x , v.y, v.z};
}

inline Quaternion Convert(const aiQuaternion& q)
{
	return { q.x , q.y, q.z , q.w};
}

inline Matrix4 Convert(const aiMatrix4x4& m)
{
	Matrix4 mat = *(reinterpret_cast<const Matrix4*>(&m));
	return Transpose(mat);
}

void ExportEmbeddedTexture(const aiTexture& texture, const Arguments& args, const std::string& fileName)
{
	printf("Extracting Embedded Texture ...\n");

	std::string fullFileName = args.outputFileName;
	fullFileName = fullFileName.substr(0, fullFileName.rfind('/') + 1);
	fullFileName += fileName;

	FILE* file = nullptr;
	fopen_s(&file, fullFileName.c_str(), "wb");
	size_t written = fwrite(texture.pcData, 1, texture.mWidth, file);
	ASSERT(written == texture.mWidth, "Error: Failed to extract embedded texture!");
	fclose(file);
}

std::string FindTexture(const aiScene& scene, const aiMaterial& inputMaterial, aiTextureType textureType, const Arguments& args, const char* suffix)
{
	std::filesystem::path textureName;

	const uint32_t textureCount = inputMaterial.GetTextureCount(textureType);
	if (textureCount > 0)
	{
		aiString texturePath;
		if (inputMaterial.GetTexture(textureType, 0, &texturePath) == aiReturn_SUCCESS)
		{
			// For FBX file, embedded textures is now accessed using GetEmbeddedTexture
			// https:://github.com/assimp/assimp/issues/1830
			if (auto embeddedTexture = scene.GetEmbeddedTexture(texturePath.C_Str()); embeddedTexture)
			{
				std::filesystem::path embeddedFilePath = texturePath.C_Str();
				std::string fileName = args.inputFileName;
				fileName.erase(fileName.length() - 4); // remove ".fbx" extension
				fileName += suffix;
				fileName += embeddedFilePath.extension().u8string();

				ExportEmbeddedTexture(*embeddedTexture, args, fileName);

				printf("Exporting embedded texture to %s\n", fileName.c_str());
				textureName = fileName;
			}
			else
			{
				std::filesystem::path filePath = texturePath.C_Str();
				std::string fileName = filePath.filename().u8string();
				printf("Adding texture %s\n", fileName.c_str());
				textureName = fileName;
			}
		}
	}

	//return textureName.filename().u8string().c_str();
	return textureName.u8string().c_str();
}

int main(int argc, char* argv[])
{
	const auto argsOpt = ParseArgs(argc, argv);
	if (!argsOpt.has_value())
	{
		PrintUsage();
		return -1;
	}

	const auto& args = argsOpt.value();

	// Create an instance of the importer class to do the parsing for us.
	Assimp::Importer importer;

	// Try to import the model into a scene.
	const aiScene* scene = importer.ReadFile(args.inputFileName,
		aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded);
	if (scene == nullptr)
	{
		printf("Error: %s\n", importer.GetErrorString());
		return -1;
	}

	// scene
	// +- mesh[_][_][_][_][_][_]...
	// +- material[_][_][_][_][_]...
	// +- animation[_][_][_][_][_]...
	//
	// +- mRootNode (scene graph)
	//     +-Node
	//       +- Node
	//       +- Node
	//          +- Node ...
	//     +-Node
	//       +- Node ...
	//     +-Node
	//     ...

	Model model;

	if (scene->HasMeshes())
	{
		printf("Reading mesh data...\n");

		const uint32_t numMeshes = scene->mNumMeshes;
		model.meshData.resize(numMeshes);

		for (uint32_t meshIndex = 0; meshIndex < numMeshes; ++meshIndex)
		{
			const aiMesh* inputMesh = scene->mMeshes[meshIndex];
			const uint32_t numVertices = inputMesh->mNumVertices;
			const uint32_t numFaces = inputMesh->mNumFaces;
			const uint32_t numIndices = numFaces * 3;

			printf("Reading Material Index...\n");
			model.meshData[meshIndex].materialIndex = inputMesh->mMaterialIndex;

			printf("Reading vertices...\n");

			std::vector<Vertex> vertices;
			vertices.reserve(numVertices);

			const aiVector3D* positions = inputMesh->mVertices;
			const aiVector3D* normals = inputMesh->mNormals;
			const aiVector3D* tangents = inputMesh->HasTangentsAndBitangents() ?inputMesh->mTangents : nullptr;
			const aiVector3D* texCoords = inputMesh->HasTextureCoords(0) ? inputMesh->mTextureCoords[0] : nullptr;

			for (uint32_t i = 0; i < numVertices; i++)
			{
				Vertex vertex;
				vertex.position = Convert(positions[i]) * args.scale;
				vertex.normal   = Convert(normals[i]); 
				vertex.tangent  = tangents ? Convert(tangents[i]) : 0.0f;
				vertex.texcoord = texCoords ? Vector2{ texCoords[i].x , texCoords[i].y} : 0.0f;
				vertices.push_back(vertex);
			}

			printf("Reading indices...\n");

			std::vector<uint32_t> indices;
			indices.reserve(numIndices);

			const aiFace* faces = inputMesh->mFaces;
			for (uint32_t i = 0; i < numFaces; ++i)
			{
				indices.push_back(faces[i].mIndices[0]);
				indices.push_back(faces[i].mIndices[1]);
				indices.push_back(faces[i].mIndices[2]);
			}

			Mesh mesh;
			mesh.vertices = std::move(vertices);
			mesh.indices = std::move(indices);
			model.meshData[meshIndex].mesh = std::move(mesh);
		}
	}

	//Look for material data.
	if (scene->HasMaterials())
	{
		printf("Reading materials ...\n");

		const uint32_t numMaterials = scene->mNumMaterials;
		model.materialData.resize(numMaterials);

		for (uint32_t i = 0; i < numMaterials; ++i)
		{
			const aiMaterial* inputMaterial = scene->mMaterials[i];

			aiColor3D ambientColor(0.0f,0.0f,0.0f);
			inputMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
			aiColor3D diffuseColor(0.0f, 0.0f, 0.0f);
			inputMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
			aiColor3D specularColor(0.0f, 0.0f, 0.0f);
			inputMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
			ai_real specularPower;
			inputMaterial->Get(AI_MATKEY_SHININESS, specularPower);

			auto& material = model.materialData[i];
			material.material.ambient = Convert(ambientColor);
			material.material.diffuse = Convert(diffuseColor);
			material.material.specular = Convert(specularColor);
			material.material.power = specularPower;
			std::string suffix = "_diffuse" + std::to_string(i);
			material.diffuseMapName = FindTexture( *scene, *inputMaterial, aiTextureType_DIFFUSE, args, suffix.c_str());
		}
	}

	SaveModel(args, model);	// ../../Assets/Models/<name>.model
	for (auto& data : model.meshData)
		data.meshBuffer.Terminate();

	printf("All done!\n");
	return 0;
}