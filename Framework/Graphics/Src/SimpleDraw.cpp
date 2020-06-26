#include "Precompiled.h"
#include "SimpleDraw.h"

#include "Camera.h"
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
			float phiIncrement = Math::Constants::Pi / rings;
			float thetaIncrement = Math::Constants::TwoPi / slices;
			std::vector<Math::Vector3> vertices;

			float phi = 0.0f;
			float theta = 0.0f;

			for (int i = 0; i <= rings; i++)
			{
				float v = static_cast<float>(i) / rings;
				float theta = 0.0f;
				for (int j = 0; j <= slices; j++)
				{
					float u = static_cast<float>(j) / (slices);
					float newRadius = radius * sinf(phi);
					Math::Vector3 vec = Math::Vector3{ newRadius* sinf(theta), radius * cosf(phi) , newRadius * -cosf(theta) };
					vertices.push_back(vec + center);
					theta += thetaIncrement;
				}
				phi += phiIncrement;
			}

			for (int y = 0; y <= rings; y++)
			{
				for (int x = 0; x < slices; x++)
				{
					if (!fill)
					{
						AddLine(vertices[(y + 1) * slices + x + 1], vertices[(y + 1) * slices + x], color);
						AddLine(vertices[y * slices + x], vertices[y * slices + x + 1], color);
						AddLine(vertices[(y + 1)* slices + x + 1], vertices[y * slices + x], color);
					}
					else
					{

						AddFace(vertices[(y + 1) * slices + x + 1], vertices[(y + 1) * slices + x], vertices[y * slices + x], color);
						AddFace(vertices[y * slices + x + 1], vertices[(y + 1)* slices + x + 1], vertices[y * slices + x], color);
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
			for (size_t i = 0; i < list.size() - 1; i++)
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
			for (size_t i = 0; i < list.size() - 1; i++)
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
		void AddOBB(const Math::OBB& obb, const Color& color)
		{
			Math::Matrix4 matTrans = Math::Matrix4::Translation(obb.center);
			Math::Matrix4 matScale = Math::Matrix4::Scaling(obb.extend);
			Math::Matrix4 matRotation = Math::Matrix4::RotationQuaternion(obb.rot);
			Math::Matrix4 toWorld = matScale * matRotation * matTrans;

			Math::Vector3 points[] =
			{
				// Front Quad
				Math::Vector3(-1.0f, -1.0f, -1.0f),
				Math::Vector3(-1.0f, +1.0f, -1.0f),
				Math::Vector3(+1.0f, +1.0f, -1.0f),
				Math::Vector3(+1.0f, -1.0f, -1.0f),

				// Back Quad
				Math::Vector3(-1.0f, -1.0f, +1.0f),
				Math::Vector3(-1.0f, +1.0f, +1.0f),
				Math::Vector3(+1.0f, +1.0f, +1.0f),
				Math::Vector3(+1.0f, -1.0f, +1.0f),
			};
			for (auto& p : points)
				p = Math::TransformCoord(p, toWorld);

			AddLine(points[0], points[1], color);
			AddLine(points[1], points[2], color);
			AddLine(points[2], points[3], color);
			AddLine(points[3], points[0], color);

			AddLine(points[0], points[4], color);
			AddLine(points[1], points[5], color);
			AddLine(points[2], points[6], color);
			AddLine(points[3], points[7], color);

			AddLine(points[4], points[5], color);
			AddLine(points[5], points[6], color);
			AddLine(points[6], points[7], color);
			AddLine(points[7], points[4], color);
		}

		void AddTransform(const Math::Matrix4& transform)
		{
			auto r = GetRight(transform);
			auto u = GetUp(transform);
			auto l = GetLook(transform);
			auto p = Math::GetTranslation(transform);
			AddLine(p, p + r, Colors::Red);
			AddLine(p, p + u, Colors::Green);
			AddLine(p, p + l, Colors::Blue);
		}
		void AddBone(const Math::Matrix4& transform, const Color& color, bool fill)
		{
			auto r = Math::GetRight(transform);
			auto u = Math::GetUp(transform);
			auto l = Math::GetLook(transform);
			auto p = Math::GetTranslation(transform);

			auto base = p;
			auto direction = r - p;
			constexpr int sectors = 4;
			auto side = Math::Normalize(Math::Cross(p, direction)) * 1.0f;
			auto angle = 0.0f;
			auto angleStep = Math::Constants::TwoPi / sectors;

			for (int i = 0; i < sectors; ++i)
			{
				auto matRot0 = Math::Matrix4::RotationAxis(direction, angle);
				auto matRot1 = Math::Matrix4::RotationAxis(direction, angle + angleStep);
				auto v0 = Math::TransformNormal(side, matRot0);
				auto v1 = Math::TransformNormal(side, matRot1);
				AddLine(base + v0, base + direction, color);
				AddLine(base + v0, base + v1, color);
				angle += angleStep;
			}
		}
		void AddBone(const Math::Vector3 & base, const Math::Vector3 & direction, float radius, const Color& color, bool fill)
		{
			constexpr int sectors = 4;
			auto side = Math::Normalize(Math::Cross(base, direction)) * radius;
			auto angle = 0.0f;
			auto angleStep = Math::Constants::TwoPi / sectors;

			auto oppDirection = -Normalize(direction) * radius;

			for (int i = 0; i < sectors; ++i)
			{
				auto matRot0 = Math::Matrix4::RotationAxis(direction, angle);
				auto matRot1 = Math::Matrix4::RotationAxis(direction, angle + angleStep);
				auto v0 = Math::TransformNormal(side, matRot0);
				auto v1 = Math::TransformNormal(side, matRot1);
				if (!fill)
				{
					AddLine(base + v0, base + direction, color);
					AddLine(base + v0, base + oppDirection, color);
					AddLine(base + v0, base + v1, color);
				}
				else
				{
					AddFace(base + v0, base + v1, base + direction, color);
					AddFace(base + v0, base + oppDirection, base + v1, color);
				}
				angle += angleStep;
			}
		}

		void AddGroundPlane(int size, bool fill, const Color & color)
		{
			const float halfSize = size * 0.5f;

			if (!fill)
			{
				for (float i = -halfSize; i <= halfSize; i += 1.0f)
				{
					AddLine({ i, 0.0f, -halfSize }, { i, 0.0f, halfSize }, color);
					AddLine({ -halfSize, 0.0f, i }, { halfSize, 0.0f, i }, color);
				}
				return;
			}

			//Filled
			std::vector<Math::Vector3> list;
			for (int y = 0; y <= size; y++)
			{
				for (int x = 0; x <= size; x++)
				{
					auto vec = Math::Vector3{ -halfSize + static_cast<float>(x) , 0.0f ,halfSize - static_cast<float>(y)};
					list.push_back(vec);
				}
			}
			for (int y = 0; y <= size; y++)
			{
				for (int x = 0; x < size; x++)
				{
					AddFace(list[(y + 1) * size + x + 1], list[(y + 1) * size + x], list[y * size + x], color);
					AddFace(list[y * size + x], list[y * size + x + 1], list[(y + 1)* size + x + 1], color);
				}
			}
		}

		void Render(const Camera& camera, const Math::Matrix4& matWorld)
		{
			auto matView = camera.GetViewMatrix();
			auto matProj = camera.GetPerspectiveMatrix();
			auto transform = Math::Transpose(matWorld * matView* matProj);
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
	ASSERT(sInstance == nullptr, "[SimpleDraw] SimpleDraw already initialized!");
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

void SimpleDraw::AddGroundPlane(int size, bool fill, const Color & color)
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

void SimpleDraw::AddOBB(const Math::OBB &obb, const Color& color)
{
	sInstance->AddOBB(obb,color);
}

void SimpleDraw::AddTransform(const Math::Matrix4 & transform)
{
	sInstance->AddTransform(transform);
}

void SimpleDraw::AddBone(const Math::Matrix4 & transform, const Color& color, bool fill)
{
	ASSERT(false, "Implementation is incorrect.");
	sInstance->AddBone(transform, color, fill);
}

void SimpleDraw::AddBone(const Math::Vector3 & position, const Math::Vector3 & direction, const Color & color, float radius, bool fill)
{
	sInstance->AddBone(position, direction, radius, color, fill);
}

void SimpleDraw::Render(const Camera & camera, const Math::Matrix4& matWorld)
{
	sInstance->Render(camera, matWorld);
}
