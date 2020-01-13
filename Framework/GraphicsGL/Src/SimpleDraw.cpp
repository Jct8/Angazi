#include "Precompiled.h"
#include "SimpleDraw.h"

#include "Shader.h"
#include "MeshBufferGL.h"
#include "VertexTypes.h"
#include "CameraGL.h"

using namespace Angazi;
using namespace Angazi::GraphicsGL;

namespace
{
	class SimpleDrawImpl
	{
	public:
		void Initialize(uint32_t maxVertexCount)
		{
			mShader.Initialize("../../Assets/Shaders/SimpleDraw.glsl");
			mMeshBuffer.Initialize<VertexPC>(nullptr, maxVertexCount, VertexPC::Format,false);
			mLineVertices = std::make_unique<VertexPC[]>(maxVertexCount);
			mVertexCount = 0;
			mMaxVertexCount = maxVertexCount;
		}
		void Terminate()
		{
			mMeshBuffer.Terminate();
			mShader.Terminate();
		}

		void AddLine(const Math::Vector3 & v0, const Math::Vector3 & v1, const Color & color)
		{
			if (mVertexCount + 2 < mMaxVertexCount)
			{
				mLineVertices[mVertexCount++] = VertexPC{ v0, color };
				mLineVertices[mVertexCount++] = VertexPC{ v1, color };
			}
		}
		void Render(const CameraGL& camera)
		{
			auto matView = camera.GetViewMatrix();
			auto matProj = camera.GetPerspectiveMatrix();
			auto transform = Math::Transpose(matView* matProj);

			mShader.Bind();

			mShader.SetUniformMat4f("WVP", transform);

			//mMeshBuffer.Update(mLineVertices.get() , mVertexCount);
			mMeshBuffer.SetTopology(MeshBufferGL::Topology::Lines);
			mMeshBuffer.Draw();

			//mVertexCount = 0;

		}

	private:
		Shader mShader;
		MeshBufferGL mMeshBuffer;
		std::unique_ptr<VertexPC[]> mLineVertices;
		uint32_t mVertexCount = 0;
		uint32_t mMaxVertexCount = 0;
	};

	std::unique_ptr<SimpleDrawImpl> sInstance;
}

void SimpleDraw::StaticInitialize(uint32_t maxVertexCount)
{
	sInstance = std::make_unique<SimpleDrawImpl>();
	sInstance->Initialize(maxVertexCount);
}

void SimpleDraw::StaticTerminate()
{
	sInstance->Terminate();
	sInstance.reset();
}

void SimpleDraw::AddLine(const Math::Vector3 & v0, const Math::Vector3 & v1, const Color & color)
{
	sInstance->AddLine(v0, v1, color);
}

void SimpleDraw::Render(const CameraGL & camera)
{
	sInstance->Render(camera);
}