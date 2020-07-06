#include "Precompiled.h"
#ifdef ENABLE_OPENGL

#include "SpriteRendererGL.h"
#include "TextureGL.h"

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	std::unique_ptr<SpriteRenderer> sSpriteRenderer = nullptr;
}

void SpriteRenderer::StaticInitialize()
{
	ASSERT(sSpriteRenderer == nullptr, "[SpriteRenderer] System already initialized!");
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
	ASSERT(sSpriteRenderer != nullptr, "[SpriteRenderer] No instance registered.");
	return sSpriteRenderer.get();
}


SpriteRenderer::~SpriteRenderer()
{
	ASSERT(sSpriteRenderer == nullptr, "[SpriteRenderer] Terminate() must be called to clean up.");
}

void SpriteRenderer::Initialize()
{
	//ASSERT(mSpriteBatch == nullptr, "[SpriteRenderer] Already initialized.");
	mTransform = Math::Matrix4::Identity;
}

void SpriteRenderer::Terminate()
{
	//ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Already Terminated.");
}

void SpriteRenderer::SetTransform(const Math::Matrix4 & transform)
{
	mTransform = transform;
}

void SpriteRenderer::BeginRender()
{
	//ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized.");
	ASSERT(false, "[SpriteRendererGL] Not implemented for OpenGL.");
}

void SpriteRenderer::EndRender()
{
	//ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized.");
	ASSERT(false, "[SpriteRendererGL] Not implemented for OpenGL.");
}

void SpriteRenderer::Draw(const Texture & texture, const Math::Vector2 & pos, float rotation, Pivot pivot, Flip flip)
{
	//ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized.");
	ASSERT(false, "[SpriteRendererGL] Not implemented for OpenGL.");
}

void SpriteRenderer::Draw(const Texture & texture, const Math::Rect & sourceRect, const Math::Vector2 & pos, float rotation, Pivot pivot, Flip flip)
{
	//ASSERT(mSpriteBatch != nullptr, "[SpriteRenderer] Not initialized.");
	ASSERT(false, "[SpriteRendererGL] Not implemented for OpenGL.");
}

#endif