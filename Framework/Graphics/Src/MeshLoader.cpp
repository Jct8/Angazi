#include "Precompiled.h"
#include "MeshLoader.h"

#include "Mesh.h"
#include "MeshManager.h"

#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags

using namespace Angazi::Graphics;
using namespace Angazi::Math;

namespace
{
	inline Color Convert(const aiColor3D& c)
	{
		return { c.r , c.g, c.b, 1.0f };
	}
	inline Vector3 Convert(const aiVector3D& v)
	{
		return { v.x , v.y, v.z };
	}
	inline Quaternion Convert(const aiQuaternion& q)
	{
		return { q.x , q.y, q.z , q.w };
	}
	inline Matrix4 Convert(const aiMatrix4x4& m)
	{
		Matrix4 mat = *(reinterpret_cast<const Matrix4*>(&m));
		return Transpose(mat);
	}
}

void MeshLoader::Load(const std::filesystem::path& filePath, float scale, std::vector<MeshId>& meshes, bool useRootPath)
{
	// Create an instance of the importer class to do the parsing for us.
	Assimp::Importer importer;

	// Try to import the model into a scene.
	std::filesystem::path rootPath = useRootPath ? MeshManager::Get()->GetRootPath() : "" ;
	const aiScene* scene = importer.ReadFile(rootPath.u8string() + filePath.u8string(), aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded);
	if (scene == nullptr)
	{
		LOG("Error: %s", importer.GetErrorString());
		return;
	}

	if (scene->HasMeshes())
	{
		const uint32_t numMeshes = scene->mNumMeshes;

		LOG("Reading mesh data...");
		for (uint32_t meshIndex = 0; meshIndex < numMeshes; ++meshIndex)
		{
			const aiMesh* inputMesh = scene->mMeshes[meshIndex];
			const uint32_t numVertices = inputMesh->mNumVertices;
			const uint32_t numFaces = inputMesh->mNumFaces;
			const uint32_t numIndices = numFaces * 3;

			//LOG("Reading Material Index...\n");
			//model.meshData[meshIndex].materialIndex = inputMesh->mMaterialIndex;

			LOG("Reading vertices...");
			std::vector<Vertex> vertices;
			vertices.reserve(numVertices);

			const aiVector3D* positions = inputMesh->mVertices;
			const aiVector3D* normals = inputMesh->mNormals;
			const aiVector3D* tangents = inputMesh->HasTangentsAndBitangents() ? inputMesh->mTangents : nullptr;
			const aiVector3D* texCoords = inputMesh->HasTextureCoords(0) ? inputMesh->mTextureCoords[0] : nullptr;

			for (uint32_t i = 0; i < numVertices; i++)
			{
				Vertex vertex;
				vertex.position = Convert(positions[i]) * scale;
				vertex.normal = Convert(normals[i]);
				vertex.tangent = tangents ? Convert(tangents[i]) : 0.0f;
				vertex.texcoord = texCoords ? Vector2{ texCoords[i].x , texCoords[i].y } : 0.0f;
				vertices.push_back(vertex);
			}

			LOG("Reading indices...");
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
			MeshId meshId = MeshManager::Get()->LoadMesh(mesh);
			meshes.emplace_back(meshId);
		}
	}
}
