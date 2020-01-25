#include "Precompiled.h"
#include "SimpleDraw.h"

#include "CameraGL.h"
#include "GraphicsSystemGL.h"
#include "MeshBufferGL.h"
#include "Shader.h"
#include "VertexTypes.h"

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
			mMeshBuffer.Initialize<VertexPC>(nullptr, maxVertexCount, VertexPC::Format,true);
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
				list.push_back(Math::Vector2{ radius*cosf(theta),radius* sinf(theta) } +center);
			}

			for (size_t i = 0; i < list.size() - 1; i++)
			{
				AddScreenLine(list[i], list[i + 1], color);
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

		void AddAABB(const Math::Vector3 & center, const Math::Vector3 & extend, const Color & color, bool fill)
		{
			if (mVertexCount + 24 >= mMaxVertexCount)
				return;
			float halfWidth = extend.y;
			float halfLength = extend.x;
			float halfHeight = extend.y;

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
					AddLine(Math::Vector3{ -halfLength,i*halfHeight,j*halfWidth } +center, Math::Vector3{ halfLength,i*halfHeight,j*halfWidth }+center, color);
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
		void AddSphere(const Math::Vector3& center, float radius, const Color& color, int slices, int rings, bool fill)
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
					list.push_back(Math::Vector3{ newRadius * cosf(theta), radius * cosf(phi) , newRadius * sinf(theta) } +center);
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
		void AddCone(float height, float radius, const Color& color, bool fill)
		{
			int slices = 16;
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
		void AddCone(const Math::Vector3& base, const Math::Vector3& direction, float height, float radius, const Color& color, bool fill = false)
		{
			int slices = 16;
			float thetaIncrement = Math::Constants::TwoPi / slices;
			std::vector<Math::Vector3> list;

			Math::Vector3 d = Math::Normalize(direction);
			float angle = acosf(Math::Dot(Math::Vector3::YAxis, d));
			Math::Vector3 rotationAxis = Math::Cross(Math::Vector3::YAxis, d);
			Math::Matrix4 rotationMatrix = Math::Matrix4::RotationAxis(rotationAxis, angle);

			for (float theta = 0; theta <= Math::Constants::TwoPi; theta += thetaIncrement)
			{
				Math::Vector3 vec3 = Math::TransformNormal(Math::Vector3{ radius * cosf(theta), 0.0f , radius * sinf(theta) }, rotationMatrix) + base;
				list.push_back(vec3);
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
					Math::Vector3 heightVec = Math::TransformNormal(Math::Vector3{ 0.0f , height , 0.0f }, rotationMatrix) + base;
					AddLine(list[i], heightVec, color);
				}
				else
				{
					Math::Vector3 heightVec = Math::TransformNormal(Math::Vector3{ 0.0f , height , 0.0f }, rotationMatrix) + base;
					AddFace(list[i], heightVec, list[i + 1], color);
					AddFace(list[i], list[i + 1], heightVec, color);
				}
			}
		}
		void AddCylinder(const Math::Vector3 & base, const Math::Vector3 & direction, float height, float radius, const Color & color, bool fill)
		{
			int slices = 16;
			int rings = 16;
			float increment = Math::Constants::TwoPi / static_cast<float>(slices);
			float ringRatio = 1.0f / rings;
			std::vector<Math::Vector3> list;

			Math::Vector3 d = Math::Normalize(direction);
			float angle = acosf(Math::Dot(d, Math::Vector3::YAxis));
			Math::Vector3 rotationAxis = Math::Cross(Math::Vector3::YAxis, d);
			Math::Matrix4 rotationMatrix = Math::Matrix4::RotationAxis(rotationAxis, angle);

			for (int y = 0; y <= rings; y++)
			{
				for (float theta = 0; theta <= Math::Constants::TwoPi; theta += increment)
				{
					Math::Vector3 vec = Math::TransformNormal(
						Math::Vector3{ sinf(theta) * radius , height * y * ringRatio , cosf(theta) * radius }, rotationMatrix) + base;
					list.push_back(vec);
				}
			}
			int sectorCount = rings + 1;
			for (int y = 0; y <= rings; y++)
			{
				for (int x = 0; x < slices; x++)
				{
					if (!fill)
					{
						AddLine(list[y * sectorCount + x], list[y * sectorCount + x + 1], color);
						if (y + 1 <= rings)
							AddLine(list[y * sectorCount + x], list[(y + 1) * sectorCount + x], color);
						if (y == 0 || y == rings)
						{
							Math::Vector3 centerVec =
								Math::TransformNormal(Math::Vector3{ 0.f, height * y * ringRatio , 0.0f }, rotationMatrix) + base;
							AddLine(list[y * sectorCount + x], centerVec, color);
						}
					}
					else
					{
						AddFace(list[(y + 1) * rings + x + 1], list[(y + 1) * rings + x], list[y * rings + x], color);
						AddFace(list[y * rings + x], list[y * rings + x + 1], list[(y + 1)* rings + x + 1], color);
						if (y == 0)
						{
							Math::Vector3 centerVec =
								Math::TransformNormal(Math::Vector3{ 0.0f , height * y * ringRatio , 0.0f }, rotationMatrix) + base;
							AddFace(list[y * sectorCount + x], centerVec, list[y * sectorCount + x + 1], color);
						}
						else if (y == rings)
						{
							Math::Vector3 centerVec =
								Math::TransformNormal(Math::Vector3{ 0.0f, height * y * ringRatio , 0.0f }, rotationMatrix) + base;
							AddFace(list[y * sectorCount + x], list[y * sectorCount + x + 1], centerVec, color);
						}
					}
				}
			}
		}

		void AddGroundPlane(float size, bool fill, const Color & color)
		{
			std::vector<Math::Vector3> list;
			for (int y = 0; y <= size; y++)
			{
				for (int x = 0; x <= size; x++)
				{
					auto vec = Math::Vector3{ -0.5f*size + static_cast<float>(x) , 0.0f ,0.5f*size - static_cast<float>(y) };
					list.push_back(vec);
				}
			}

			for (int y = 0; y <= size; y++)
			{
				for (int x = 0; x < size; x++)
				{
					if (!fill)
					{
						AddLine(list[y * size + x], list[y * size + x + 1], color);
						AddLine(list[(y + 1) * size + x], list[y * size + x], color);
					}
					else
					{
						AddFace(list[(y + 1) * size + x + 1], list[(y + 1) * size + x], list[y * size + x], color);
						AddFace(list[y * size + x], list[y * size + x + 1], list[(y + 1)* size + x + 1], color);
					}
				}
			}
		}

		void Render(const CameraGL& camera)
		{
			auto matView = camera.GetViewMatrix();
			auto matProj = camera.GetPerspectiveMatrix();
			auto transform = Math::Transpose(matView* matProj);

			mShader.Bind();

			mShader.SetUniformMat4f("WVP", transform);

			//Draw 3D Lines
			mMeshBuffer.Update(mLineVertices.get() , mVertexCount);
			mMeshBuffer.SetTopology(MeshBufferGL::Topology::Lines);
			mMeshBuffer.Draw();

			//Draw 3D Filled
			mMeshBuffer.Update(mFillVertices.get(), mFillVertexCount);
			mMeshBuffer.SetTopology(MeshBufferGL::Topology::Triangles);
			mMeshBuffer.Draw();

			//Draw 2D Lines
			auto system = GraphicsGL:: GraphicsSystemGL::Get();
			const uint32_t w = system->GetBackBufferWidth();
			const uint32_t h = system->GetBackBufferHeight();
			Math::Matrix4 screenToNDC
			{
				2.0f / w ,  0.0f	, 0.0f , 0.0f,
				0.0f	 , -2.0f / h, 0.0f , 0.0f,
				0.0f	 ,  0.0f	, 1.0f , 0.0f,
				-1.0f	 ,  1.0f	, 0.0f , 1.0f
			};
			mShader.SetUniformMat4f("WVP", Math::Transpose(screenToNDC));
			mMeshBuffer.Update(m2DLineVertices.get(), m2DVertexCount);
			mMeshBuffer.SetTopology(MeshBufferGL::Topology::Lines);
			mMeshBuffer.Draw();

			mVertexCount = 0;
			m2DVertexCount = 0;
			mFillVertexCount = 0;

		}

	private:
		Shader mShader;
		MeshBufferGL mMeshBuffer;
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

void SimpleDraw::AddGroundPlane(float size, bool fill, const Color & color)
{
	sInstance->AddGroundPlane(size, fill, color);
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
	sInstance->AddScreenCircle({ centerX,centerY }, radius, color);
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
	Math::Rect rect(left, top, right, bottom);
	sInstance->AddScreenRect(rect, color);
}

void SimpleDraw::AddAABB(const Math::Vector3 & center, const Math::Vector3 & extend, const Color & color, bool fill)
{
	sInstance->AddAABB(center, extend, color, fill);
}

void SimpleDraw::AddCone(float height, float radius, const Color& color, bool fill)
{
	sInstance->AddCone(height, radius, color, fill);
}

void SimpleDraw::AddCone(const Math::Vector3 & base, const Math::Vector3 & direction, float height, float radius, const Color & color, bool fill)
{
	sInstance->AddCone(base, direction, height, radius, color, fill);
}

void SimpleDraw::AddCylinder(const Math::Vector3 & base, const Math::Vector3 & direction, float height, float radius, const Color & color, bool fill)
{
	sInstance->AddCylinder(base, direction, height, radius, color, fill);
}

void SimpleDraw::AddSphere(const Math::Vector3& center, float radius, const Color& color, bool fill, int slices, int rings)
{
	sInstance->AddSphere(center, radius, color, slices, rings, fill);
}

void SimpleDraw::AddSphere(const Math::Sphere & sphere, const Color & color, bool fill, int slices, int rings)
{
	sInstance->AddSphere(sphere.center, sphere.radius, color, slices, rings, fill);
}

void SimpleDraw::AddSphere(float x, float y, float z, float radius, const Color & color, bool fill, int slices, int rings)
{
	sInstance->AddSphere({ x,y,z }, radius, color, slices, rings, fill);
}

void SimpleDraw::Render(const CameraGL & camera)
{
	sInstance->Render(camera);
}