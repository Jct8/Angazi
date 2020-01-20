#include "Precompiled.h"
#include "SimpleDraw.h"

#include "Camera.h"
#include "ConstantBuffer.h"
#include "GraphicsSystem.h"
#include "MeshBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "VertexTypes.h"

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
			m2DLineVertices = std::make_unique<VertexPC[]>(maxVertexCount);
			mFillVertices = std::make_unique<VertexPC[]>(maxVertexCount);
			mVertexCount = 0;
			m2DVertexCount = 0;
			mFillVertexCount = 0;
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
			if (mFillVertexCount + 3 < mMaxVertexCount)
			{
				mFillVertices[mFillVertexCount++] = VertexPC{ v0, color };
				mFillVertices[mFillVertexCount++] = VertexPC{ v1, color };
				mFillVertices[mFillVertexCount++] = VertexPC{ v2, color };
			}
		}

		void AddScreenLine(const Math::Vector2 & v0, const Math::Vector2 & v1, const Color & color)
		{
			if (m2DVertexCount + 2 < mMaxVertexCount)
			{
				m2DLineVertices[m2DVertexCount++] = VertexPC{ Math::Vector3(v0.x, v0.y, 0.0f), color };
				m2DLineVertices[m2DVertexCount++] = VertexPC{ Math::Vector3(v1.x, v1.y, 0.0f), color };
			}
		}
		void AddScreenCircle(const Math::Vector2& center, float radius, const Color & color)
		{
			if (m2DVertexCount + 32 >= mMaxVertexCount)
				return;
			int slices = 16;
			float thetaIncrement = Math::Constants::TwoPi / slices;

			std::vector<Math::Vector2> list;
			for (float theta = 0; theta <= Math::Constants::TwoPi; theta += thetaIncrement)
			{
				list.push_back(Math::Vector2{ radius*cosf(theta),radius* sinf(theta) } + center);
			}

			for (size_t i = 0; i < list.size() -1; i++)
			{
				AddScreenLine(list[i],list[i+1],color);
			}

		}
		void AddScreenRect(const Math::Rect & rect, const Color & color)
		{
			if (m2DVertexCount + 8 >= mMaxVertexCount)
				return;
			AddScreenLine({ rect.left , rect.top }, { rect.right , rect.top }, color);
			AddScreenLine({ rect.left , rect.bottom }, { rect.right , rect.bottom }, color);
			AddScreenLine({ rect.left , rect.top }, { rect.left , rect.bottom }, color);
			AddScreenLine({ rect.right , rect.top }, { rect.right , rect.bottom }, color);
		}

		void AddBox(const Math::Vector3& center, float width, float length, float height, const Color & color, bool fill)
		{
			if (mVertexCount + 24 >= mMaxVertexCount)
				return;
			float halfWidth = width * 0.5f;
			float halfLength = length * 0.5f;
			float halfHeight = height * 0.5f;

			if (fill)
			{
				//Front
				AddFace(Math::Vector3{ -halfLength, halfHeight, -halfWidth }+center, Math::Vector3{ halfLength, -halfHeight, -halfWidth }+center, Math::Vector3{ -halfLength, -halfHeight, -halfWidth }+center, color);
				AddFace(Math::Vector3{ -halfLength, halfHeight, -halfWidth }+center, Math::Vector3{ halfLength,  halfHeight, -halfWidth }+center, Math::Vector3{ halfLength, -halfHeight, -halfWidth }+center, color);
				//Back
				AddFace(Math::Vector3{ -halfLength, halfHeight,  halfWidth }+center, Math::Vector3{ -halfLength, -halfHeight,  halfWidth }+center, Math::Vector3{ halfLength, -halfHeight,  halfWidth }+center, color);
				AddFace(Math::Vector3{ -halfLength, halfHeight,  halfWidth }+center, Math::Vector3{ halfLength, -halfHeight,  halfWidth }+center, Math::Vector3{ halfLength,  halfHeight,  halfWidth }+center, color);
				//Left
				AddFace(Math::Vector3{ -halfLength, halfHeight, -halfWidth }+center, Math::Vector3{ -halfLength, -halfHeight, -halfWidth }+center, Math::Vector3{ -halfLength, -halfHeight,  halfWidth }+center, color);
				AddFace(Math::Vector3{ -halfLength, halfHeight, -halfWidth }+center, Math::Vector3{ -halfLength, -halfHeight,  halfWidth }+center, Math::Vector3{ -halfLength,  halfHeight,  halfWidth }+center, color);
				//Right
				AddFace(Math::Vector3{ halfLength, halfHeight, -halfWidth }+center, Math::Vector3{ halfLength, -halfHeight,  halfWidth }+center, Math::Vector3{ halfLength, -halfHeight, -halfWidth }+center, color);
				AddFace(Math::Vector3{ halfLength, halfHeight, -halfWidth }+center, Math::Vector3{ halfLength,  halfHeight,  halfWidth }+center, Math::Vector3{ halfLength, -halfHeight,  halfWidth }+center, color);
				//Top
				AddFace(Math::Vector3{ -halfLength, halfHeight, -halfWidth }+center, Math::Vector3{ -halfLength,  halfHeight,  halfWidth }+center, Math::Vector3{ halfLength,  halfHeight,  halfWidth }+center, color);
				AddFace(Math::Vector3{ -halfLength, halfHeight, -halfWidth }+center, Math::Vector3{ halfLength,  halfHeight,  halfWidth }+center, Math::Vector3{ halfLength,  halfHeight, -halfWidth }+center, color);
				//Bottom
				AddFace(Math::Vector3{ -halfLength, -halfHeight, -halfWidth }+center, Math::Vector3{ halfLength, -halfHeight,  halfWidth }+center, Math::Vector3{ -halfLength, -halfHeight,  halfWidth }+center, color);
				AddFace(Math::Vector3{ -halfLength, -halfHeight, -halfWidth }+center, Math::Vector3{ halfLength, -halfHeight, -halfWidth }+center, Math::Vector3{ halfLength, -halfHeight,  halfWidth }+center, color);
				return;
			}

			for (int i = -1; i <= 1; i += 2)
			{
				for (int j = -1; j <= 1; j += 2)
				{
					AddLine(Math::Vector3{ -halfLength,i*halfHeight,j*halfWidth } + center, Math::Vector3{ halfLength,i*halfHeight,j*halfWidth }+center, color);
				}
				for (int j = -1; j <= 1; j += 2)
				{
					AddLine(Math::Vector3{ i*halfLength,-halfHeight,j*halfWidth }+center, Math::Vector3{ i*halfLength, halfHeight,j*halfWidth }+center, color);
				}
				for (int j = -1; j <= 1; j += 2)
				{
					AddLine(Math::Vector3{ i*halfLength,j*halfHeight, -halfWidth }+center, Math::Vector3{ i*halfLength,j*halfHeight,  halfWidth }+center, color);
				}
			}
		}
		void AddSphere(const Math::Vector3& center, float radius, const Color& color, float slices, float rings, bool fill)
		{
			float thetaIncrement = Math::Constants::TwoPi / rings;
			float phiIncrement = Math::Constants::Pi / slices;
			std::vector<Math::Vector3> list;

			for (float phi = 0; phi <= Math::Constants::Pi + phiIncrement; phi += phiIncrement)
			{
				for (float theta = 0; theta <= Math::Constants::TwoPi; theta += thetaIncrement)
				{
					float u = theta / Math::Constants::TwoPi;
					float v = static_cast<float>(phi) / static_cast<float>(Math::Constants::Pi);
					float newRadius = radius * sinf(phi);
					list.push_back(Math::Vector3{ newRadius * cosf(theta), radius * cosf(phi) , newRadius * sinf(theta) } + center);
				}
			}

			if (mVertexCount + list.size() - 1 > mMaxVertexCount)
			{
				return;
			}

			for (int y = 0; y <= slices; y++)
			{
				for (int x = 0; x <= rings; x++)
				{
					if (!fill)
					{
						AddLine(list[(y + 1) * slices + x + 1], list[(y + 1) * slices + x], color);
						AddLine(list[y * slices + x], list[y * slices + x + 1], color);
						AddLine(list[(y + 1)* slices + x + 1], list[y * slices + x], color);
					}
					else
					{

						AddFace(list[(y + 1) * slices + x + 1], list[(y + 1) * slices + x], list[y * slices + x], color);
						AddFace(list[y * slices + x + 1], list[(y + 1)* slices + x + 1], list[y * slices + x], color);
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
					AddLine(list[i], list[i + 1], color);
					AddLine(list[i], { 0.0f , height , 0.0f }, color);
				}
				else
				{
					AddFace(list[i], { 0.0f , height , 0.0f }, list[i + 1], color);
					AddFace(list[i], list[i + 1], Math::Vector3::Zero, color);
				}
			}
		}
		void Render(const Camera& camera)
		{
			auto matView = camera.GetViewMatrix();
			auto matProj = camera.GetPerspectiveMatrix();
			auto transform = Math::Transpose(matView* matProj);
			mConstantBuffer.Update(&transform);
			mConstantBuffer.BindVS();

			mVertexShader.Bind();
			mPixelShader.Bind();

			//Draw 3D Lines
			mMeshBuffer.Update(mLineVertices.get(), mVertexCount);
			mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
			mMeshBuffer.Draw();

			//Draw 3D Filled
			mMeshBuffer.Update(mFillVertices.get(), mFillVertexCount);
			mMeshBuffer.SetTopology(MeshBuffer::Topology::Triangles);
			mMeshBuffer.Draw();

			//Draw 2D Lines
			auto system = GraphicsSystem::Get();
			const uint32_t w = system->GetBackBufferWidth();
			const uint32_t h = system->GetBackBufferHeight();
			Math::Matrix4 screenToNDC
			{
				2.0f / w ,  0.0f	, 0.0f , 0.0f,
				0.0f	 , -2.0f / h, 0.0f , 0.0f,
				0.0f	 ,  0.0f	, 1.0f , 0.0f,
				-1.0f	 ,  1.0f	, 0.0f , 1.0f
			};
			mConstantBuffer.Update(&Math::Transpose(screenToNDC));
			mConstantBuffer.BindVS();

			mMeshBuffer.Update(m2DLineVertices.get(), m2DVertexCount);
			mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
			mMeshBuffer.Draw();

			mVertexCount = 0;
			m2DVertexCount = 0;
			mFillVertexCount = 0;

		}

	private:
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		ConstantBuffer mConstantBuffer;
		MeshBuffer mMeshBuffer;
		std::unique_ptr<VertexPC[]> mLineVertices;
		std::unique_ptr<VertexPC[]> m2DLineVertices;
		std::unique_ptr<VertexPC[]> mFillVertices;
		uint32_t mVertexCount = 0;
		uint32_t m2DVertexCount = 0;
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

void SimpleDraw::AddScreenLine(const Math::Vector2 & v0, const Math::Vector2 & v1, const Color & color)
{
	sInstance->AddScreenLine(v0, v1, color);
}

void SimpleDraw::AddScreenLine(float x0, float y0, float x1, float y1, const Color& color)
{
	sInstance->AddScreenLine({ x0, y0 }, { x1, y1 }, color);
}

void SimpleDraw::AddScreenCircle(const Math::Circle & circle, const Color & color)
{
	sInstance->AddScreenCircle(circle.center, circle.radius, color);
}

void SimpleDraw::AddScreenCircle(const Math::Vector2 & center, float radius, const Color & color)
{
	sInstance->AddScreenCircle(center, radius, color);
}

void SimpleDraw::AddScreenCircle(float centerX, float centerY, float radius, const Color & color)
{
	sInstance->AddScreenCircle({centerX,centerY}, radius, color);
}

void SimpleDraw::AddScreenRect(const Math::Rect & rect, const Color & color)
{
	sInstance->AddScreenRect(rect, color);
}

void SimpleDraw::AddScreenRect(const Math::Vector2 & min, const Math::Vector2 & max, const Color & color)
{
	Math::Rect rect(min.x, min.y, max.x, max.y);
	sInstance->AddScreenRect(rect, color);
}

void SimpleDraw::AddScreenRect(float left, float top, float right, float bottom, const Color & color)
{
	Math::Rect rect(left,top, right, bottom);
	sInstance->AddScreenRect(rect, color);
}

void SimpleDraw::AddBox(const Math::Vector3& center, float width, float length, float height, const Color & color, bool fill)
{
	sInstance->AddBox(center,width, length, height, color, fill);
}

void SimpleDraw::AddCone(float height, float radius, const Color& color, bool fill, int slices)
{
	sInstance->AddCone(height, radius, color, slices, fill);
}

void SimpleDraw::AddSphere(const Math::Vector3& center, float radius, const Color& color, bool fill, int slices, int rings)
{
	sInstance->AddSphere(center, radius, color, slices, rings, fill);
}

void SimpleDraw::AddSphere(const Math::Sphere & sphere, const Color & color, bool fill, uint32_t slices, uint32_t rings)
{
	sInstance->AddSphere(sphere.center, sphere.radius, color, slices, rings, fill);
}

void SimpleDraw::AddSphere(float x, float y, float z, float radius, const Color & color,bool fill , uint32_t slices, uint32_t rings)
{
	sInstance->AddSphere({x,y,z}, radius, color, slices, rings, fill);
}

void SimpleDraw::Render(const Camera & camera)
{
	sInstance->Render(camera);
}