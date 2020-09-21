#include "Precompiled.h"
#include "SpriteRendererGL.h"

#ifdef ENABLE_OPENGL

#include "TextureGL.h"
#include "Graphics/Inc/MeshBuilder.h"

using namespace Angazi;
using namespace Angazi::Math;
using namespace Angazi::Graphics;

namespace
{
	const auto positionOriginMat = Math::Matrix4::Translation({ -0.5f,-0.5f, 0.0f });
	const auto positionOriginBackMat = Math::Matrix4::Translation({ 0.5f,0.5f, 0.0f });

	Vector3 GetOrigin(uint32_t width, uint32_t height, Pivot pivot)
	{
		auto index = static_cast<std::underlying_type_t<Pivot>>(pivot);
		constexpr Vector2 offsets[] =
		{
			{ 0.0f, 0.0f }, // TopLeft
			{ 0.5f, 0.0f }, // Top
			{ 1.0f, 0.0f }, // TopRight
			{ 0.0f, 0.5f }, // Left
			{ 0.5f, 0.5f }, // Center
			{ 1.0f, 0.5f }, // Right
			{ 0.0f, 1.0f }, // BottomLeft
			{ 0.5f, 1.0f }, // Bottom
			{ 1.0f, 1.0f }, // BottomRight
		};
		return { width * offsets[index].x, height * offsets[index].y, 0.0f};
	}

	std::unique_ptr<SpriteRenderer> sSpriteRenderer = nullptr;
}

void SpriteRenderer::StaticInitialize()
{
	ASSERT(sSpriteRenderer == nullptr, "SpriteRendererGL -- System already initialized!");
	sSpriteRenderer = std::make_unique<SpriteRenderer>();
	sSpriteRenderer->Initialize();
}

void SpriteRenderer::StaticTerminate()
{
	if (sSpriteRenderer != nullptr)
	{
		sSpriteRenderer->Terminate();
		sSpriteRenderer.reset();
	}
}

SpriteRenderer * Angazi::Graphics::SpriteRenderer::Get()
{
	ASSERT(sSpriteRenderer != nullptr, "SpriteRenderer -- No instance registered.");
	return sSpriteRenderer.get();
}


SpriteRenderer::~SpriteRenderer()
{
	ASSERT(sSpriteRenderer == nullptr, "[SpriteRenderer] Terminate() must be called to clean up.");
}

void SpriteRenderer::Initialize()
{
	mTransform = Math::Matrix4::Identity;

	mPixelShader.Initialize("../../Assets/GLShaders/Sprite.glsl");
	mVertexShader.Initialize("../../Assets/GLShaders/Sprite.glsl", VertexPX::Format);
	mConstantBuffer.Initialize(sizeof(Math::Matrix4));

	MeshPX mesh;
	mesh.vertices.push_back({ {-0.5f, 0.5f,0.0f} , {0.0f, 1.0f} });
	mesh.vertices.push_back({ { 0.5f,-0.5, 0.0f} , {1.0f, 0.0f} });
	mesh.vertices.push_back({ {-0.5f,-0.5f,0.0f} , {0.0f, 0.0f} });

	mesh.vertices.push_back({ {-0.5f, 0.5f,0.0f} , {0.0f, 1.0f} });
	mesh.vertices.push_back({ { 0.5f, 0.5f,0.0f} , {1.0f, 1.0f} });
	mesh.vertices.push_back({ { 0.5f,-0.5,0.0f}  , {1.0f, 0.0f} });

	mQuadMeshBuffer.Initialize(mesh);
	mBlendState.Initialize(BlendState::Mode::AlphaBlend);
}

void SpriteRenderer::Terminate()
{
	mBlendState.Terminate();
	mQuadMeshBuffer.Terminate();
	mConstantBuffer.Terminate();
	mVertexShader.Terminate();
	mPixelShader.Terminate();
}

void SpriteRenderer::SetTransform(const Math::Matrix4 & transform)
{
	mTransform = transform;
}

void SpriteRenderer::BeginRender()
{
	glDisable(GL_DEPTH_TEST);
	mVertexShader.Bind();
	mPixelShader.Bind();
	mBlendState.Bind();
}

void SpriteRenderer::EndRender()
{
	mBlendState.ClearState();
	glEnable(GL_DEPTH_TEST);
}

void SpriteRenderer::Draw(const Texture & texture, const Math::Vector2 & pos, float rotation, Pivot pivot, Flip flip)
{
	ASSERT(sSpriteRenderer != nullptr, "SpriteRenderer -- Not initialized.");
	Draw(texture, {}, pos, rotation, pivot, flip);
}

void SpriteRenderer::Draw(const Texture & texture, const Math::Rect & sourceRect, const Math::Vector2 & pos, float rotation, Pivot pivot, Flip flip)
{
	// TODO : Fix sourceRect
	ASSERT(sSpriteRenderer != nullptr, "SpriteRenderer -- Not initialized.");

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

	// Get Pivot Point
	auto width = texture.GetWidth();
	auto height = texture.GetHeight();
	auto origin = GetOrigin(width, height, pivot);
	auto positionMat = Math::Matrix4::Translation({ pos.x - origin.x,pos.y - origin.y, 0.0f });
	auto scaleMat = Matrix4::Scaling({ (float) width , (float) height, 0.0f });

	auto rotMat = Math::Matrix4::RotationZ(rotation);
	switch (flip)
	{
	case Angazi::Graphics::Flip::Horizontal:
		rotMat = rotMat * Matrix4::RotationY(Constants::Pi);
		break;
	case Angazi::Graphics::Flip::Vertical:
		rotMat = rotMat * Matrix4::RotationX(Constants::Pi);
		break;
	case Angazi::Graphics::Flip::Both:
		rotMat = rotMat * Matrix4::RotationY(Constants::Pi) * Matrix4::RotationX(Constants::Pi);
		break;
	}

	auto finalMat =  rotMat * positionOriginBackMat * scaleMat * positionMat;

	mTransform = Math::Transpose(finalMat * screenToNDC);
	mConstantBuffer.Update(&mTransform);
	mConstantBuffer.BindVS();
	texture.BindPS();

	mQuadMeshBuffer.Draw();
}

#endif