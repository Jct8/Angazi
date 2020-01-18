#include "Precompiled.h"
#include "SimpleDraw.h"

#include "PixelShader.h"
#include "VertexShader.h"
#include "ConstantBuffer.h"
#include "MeshBuffer.h"
#include "VertexTypes.h"
#include "Camera.h"

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	class SimpleDrawImpl
	{
	public:
		void Initialize(uint32_t maxVertexCount)
		{
			mVertexShader.Initialize("../../Assets/Shaders/SimpleDraw.fx", VertexPC::Format);
			mPixelShader.Initialize("../../Assets/Shaders/SimpleDraw.fx");
			mConstantBuffer.Initialize(sizeof(Math::Matrix4));
			mMeshBuffer.Initialize<VertexPC>(nullptr, maxVertexCount, true);
			mLineVertices = std::make_unique<VertexPC[]>(maxVertexCount);
			mFillVertices = std::make_unique<VertexPC[]>(maxVertexCount);
			mVertexCount = 0;
			mMaxVertexCount = maxVertexCount;
		}
		void Terminate()
		{
			mMeshBuffer.Terminate();
			mConstantBuffer.Terminate();
			mPixelShader.Terminate();
			mVertexShader.Terminate();
		}

		void AddLine(const Math::Vector3 & v0, const Math::Vector3 & v1, const Color & color)
		{
			if (mVertexCount + 2 < mMaxVertexCount)
			{
				mLineVertices[mVertexCount++] = VertexPC{ v0, color };
				mLineVertices[mVertexCount++] = VertexPC{ v1, color };
			}
		}

		void AddFace(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector3& v2, const Color& color)
		{
			if (mVertexCount + 3 < mMaxVertexCount)
			{
				mFillVertices[mFillVertexCount++] = VertexPC{ v0, color };
				mFillVertices[mFillVertexCount++] = VertexPC{ v1, color };
				mFillVertices[mFillVertexCount++] = VertexPC{ v2, color };
			}
		}

		void AddScreenCircle(Math::Vector2 center, float radius, const Color & color)
		{

		}

		void AddBox(float width, float length, float height, const Color & color, bool fill)
		{
			if (mVertexCount + 24 >= mMaxVertexCount)
			{
				return;
			}
			float halfWidth = width * 0.5f;
			float halfLength = length * 0.5f;
			float halfHeight = height * 0.5f;
			for (int i = -1; i <= 1; i += 2)
			{
				for (int j = -1; j <= 1; j += 2)
				{
					if (!fill)
					{
						mLineVertices[mVertexCount++] = VertexPC{ {-halfLength,i*halfHeight,j*halfWidth}, color };
						mLineVertices[mVertexCount++] = VertexPC{ { halfLength,i*halfHeight,j*halfWidth}, color };
					}
					else
					{
						mFillVertices[mFillVertexCount++] = VertexPC{ {-halfLength,i*halfHeight,j*halfWidth}, color };
						mFillVertices[mFillVertexCount++] = VertexPC{ { halfLength,i*halfHeight,j*halfWidth}, color };
					}
				}
				for (int j = -1; j <= 1; j += 2)
				{
					if (!fill)
					{

						mLineVertices[mVertexCount++] = VertexPC{ {i*halfLength,-halfHeight,j*halfWidth}, color };
						mLineVertices[mVertexCount++] = VertexPC{ {i*halfLength, halfHeight,j*halfWidth}, color };
					}
					else
					{
						mFillVertices[mFillVertexCount++] = VertexPC{ {i*halfLength,-halfHeight,j*halfWidth}, color };
						mFillVertices[mFillVertexCount++] = VertexPC{ {i*halfLength, halfHeight,j*halfWidth}, color };
					}
				}
				for (int j = -1; j <= 1; j += 2)
				{
					if (!fill)
					{
						mLineVertices[mVertexCount++] = VertexPC{ {i*halfLength,j*halfHeight, -halfWidth}, color };
						mLineVertices[mVertexCount++] = VertexPC{ {i*halfLength,j*halfHeight,  halfWidth}, color };
					}
					else
					{
						mFillVertices[mFillVertexCount++] = VertexPC{ {i*halfLength,j*halfHeight, -halfWidth}, color };
						mFillVertices[mFillVertexCount++] = VertexPC{ {i*halfLength,j*halfHeight,  halfWidth}, color };
					}
				}
			}
		}
		void AddSphere(float radius, const Color& color, float slices, float rings, bool fill)
		{
			float thetaIncrement = Math::Constants::TwoPi / rings;
			float phiIncrement = Math::Constants::Pi / slices;
			std::vector<Math::Vector3> list;

			for (float phi = 0; phi <= Math::Constants::Pi; phi += phiIncrement)
			{
				for (float theta = 0; theta <= Math::Constants::TwoPi; theta += thetaIncrement)
				{
					float u = theta / Math::Constants::TwoPi;
					float v = static_cast<float>(phi) / static_cast<float>(Math::Constants::Pi);
					float newRadius = radius * sinf(phi);
					list.push_back(Math::Vector3{ newRadius * cosf(theta), radius * cosf(phi) , newRadius * sinf(theta) });
				}
			}

			if (mVertexCount + list.size() - 1 > mMaxVertexCount)
			{
				return;
			}

			for (int y = 0; y <= slices; y++)
			{
				for (int x = 0; x < rings; x++)
				{
					if (!fill)
					{
						AddLine(list[(y + 1) * slices + x + 1], list[(y + 1) * slices + x], color);
						AddLine(list[y * slices + x], list[y * slices + x + 1], color);
						AddLine(list[(y + 1)* slices + x + 1], list[y * slices + x], color);


						/*mLineVertices[mVertexCount++] = VertexPC{ list[(y + 1) * slices + x + 1],color };
						mLineVertices[mVertexCount++] = VertexPC{ list[(y + 1) * slices + x],color };

						mLineVertices[mVertexCount++] = VertexPC{ list[y * slices + x],color };
						mLineVertices[mVertexCount++] = VertexPC{ list[y * slices + x + 1],color };

						mLineVertices[mVertexCount++] = VertexPC{ list[(y + 1)* slices + x + 1],color };
						mLineVertices[mVertexCount++] = VertexPC{ list[y * slices + x],color };*/
					}
					else
					{

						AddFace(list[(y + 1) * slices + x + 1], list[(y + 1) * slices + x], list[y * slices + x], color);
						AddFace(list[y * slices + x + 1], list[(y + 1)* slices + x + 1], list[y * slices + x], color);

						/*mFillVertices[mFillVertexCount++] = VertexPC{ list[(y + 1) * slices + x + 1],color };
						mFillVertices[mFillVertexCount++] = VertexPC{ list[(y + 1) * slices + x],color };
						mFillVertices[mFillVertexCount++] = VertexPC{ list[y * slices + x],color };*/

						/*mFillVertices[mFillVertexCount++] = VertexPC{ list[y * slices + x + 1],color };
						mFillVertices[mFillVertexCount++] = VertexPC{ list[(y + 1)* slices + x + 1],color };
						mFillVertices[mFillVertexCount++] = VertexPC{ list[y * slices + x],color };*/
					}



				}
			}
		}
		void AddCone(float height, float radius, const Color& color, int slices, bool fill)
		{
			float thetaIncrement = Math::Constants::TwoPi / slices;
			std::vector<Math::Vector3> list;
			for (float theta = 0; theta <= Math::Constants::TwoPi; theta += thetaIncrement)
			{
				list.push_back({ radius * cosf(theta), 0.0f , radius * sinf(theta) });
			}
			if (mVertexCount + list.size() - 1 > mMaxVertexCount)
			{
				return;
			}
			for (int i = 0; i < list.size() - 1; i++)
			{
				if (!fill)
				{
					mLineVertices[mVertexCount++] = VertexPC{ list[i], color };
					mLineVertices[mVertexCount++] = VertexPC{ list[i + 1], color };

					mLineVertices[mVertexCount++] = VertexPC{ list[i], color };
					mLineVertices[mVertexCount++] = VertexPC{ { 0.0f , height , 0.0f }, color };
				}
				else
				{
					mFillVertices[mFillVertexCount++] = VertexPC{ list[i], color };
					mFillVertices[mFillVertexCount++] = VertexPC{ list[i + 1], color };

					mFillVertices[mFillVertexCount++] = VertexPC{ list[i], color };
					mFillVertices[mFillVertexCount++] = VertexPC{ { 0.0f , height , 0.0f }, color };
				}
			}
		}
		void Render(const Camera& camera)
		{
			auto matView = camera.GetViewMatrix();
			auto matProj = camera.GetPerspectiveMatrix();
			auto transform = Math::Transpose(matView* matProj);
			mConstantBuffer.Set(&transform);
			mConstantBuffer.Bind();

			mVertexShader.Bind();
			mPixelShader.Bind();

			mMeshBuffer.Update(mLineVertices.get(), mVertexCount);
			mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
			mMeshBuffer.Draw();

			mMeshBuffer.Update(mFillVertices.get(), mFillVertexCount);
			mMeshBuffer.SetTopology(MeshBuffer::Topology::Triangles);
			mMeshBuffer.Draw();

			/*auto system 
			Math::Matrix4 screenToNDC;
			
			const 
			
			screenToNDC._11 = 2.0f / screen
			auto transform = Math::Transpose(matView* matProj);
			mConstantBuffer.Set(&transform);*/

			mVertexCount = 0;
			mFillVertexCount = 0;

		}

	private:
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		ConstantBuffer mConstantBuffer;
		MeshBuffer mMeshBuffer;
		std::unique_ptr<VertexPC[]> mLineVertices;
		std::unique_ptr<VertexPC[]> mFillVertices;
		uint32_t mVertexCount = 0;
		uint32_t mFillVertexCount = 0;
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

void SimpleDraw::AddLine(const Math::Vector2 & v0, const Math::Vector2 & v1, const Color & color)
{
	sInstance->AddLine({ v0.x, v0.x , 0.0f }, { v1.x , v1.y , 0.0f }, color);
}

void SimpleDraw::AddScreenLine(float x0, float y0, float x1, float y1, const Color& color)
{
	sInstance->AddLine({ x0, y0, 0.0f }, { x1, y1 , 0.0f }, color);
}

void SimpleDraw::AddScreenCircle(Math::Vector2 center, float radius, const Color & color)
{
	sInstance->AddScreenCircle(center, radius, color);
}

void SimpleDraw::AddBox(float width, float length, float height, const Color & color, bool fill)
{
	sInstance->AddBox(width, length, height, color, fill);
}

void SimpleDraw::AddCone(float height, float radius, const Color& color, bool fill, int slices)
{
	sInstance->AddCone(height, radius, color, slices, fill);
}

void SimpleDraw::AddSphere(float radius, const Color& color, bool fill, int slices, int rings)
{
	sInstance->AddSphere(radius, color, slices, rings, fill);
}

void SimpleDraw::Render(const Camera & camera)
{
	sInstance->Render(camera);
}