#include "Precompiled.h"
#include "RenderTargetGL.h"

#ifdef ENABLE_OPENGL

#include "GraphicsSystemGL.h"

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	std::pair<uint32_t, uint32_t> GetFormats(RenderTarget::Format format)
	{
		switch (format)
		{
		case Angazi::Graphics::RenderTarget::Format::RGBA_U8:	return { GL_RGBA, GL_RGBA8 };
		case Angazi::Graphics::RenderTarget::Format::RGBA_F16:	return { GL_RGBA, GL_RGBA16F };
		case Angazi::Graphics::RenderTarget::Format::RGBA_U32:	return { GL_RGBA_INTEGER, GL_RGBA32UI };
		case Angazi::Graphics::RenderTarget::Format::R_F16:		return { GL_RED, GL_R16F };
		case Angazi::Graphics::RenderTarget::Format::R_S32:		return { GL_RED_INTEGER, GL_R32I};
		default:
			ASSERT(false, "[RenderTarget] Unsupported format %d", static_cast<uint32_t>(format));
			break;
		}
		return { GL_RGBA, GL_RGBA8 };
	};
}

RenderTarget::~RenderTarget()
{
	ASSERT(!glIsTexture(mShaderResource), "[RenderTargetGL] Terminate() must be called to clean up!");
	ASSERT(!glIsFramebuffer(mRenderTarget), "[RenderTargetGL] Terminate() must be called to clean up!");
	ASSERT(!glIsTexture(mDepthStencil), "[RenderTargetGL] Terminate() must be called to clean up!");
	//ASSERT(!glIsRenderbuffer(mDepthStencil), "[RenderTargetGL] Terminate() must be called to clean up!");
}

void RenderTarget::Initialize(uint32_t width, uint32_t height, Format format)
{
	auto textureFormat = GetFormats(format);

	glBindTexture(GL_TEXTURE_2D, 0);

	glCreateFramebuffers(1, &mRenderTarget);
	glBindFramebuffer(GL_FRAMEBUFFER, mRenderTarget);

	glCreateTextures(GL_TEXTURE_2D, 1, &mShaderResource);
	glBindTexture(GL_TEXTURE_2D, mShaderResource);
	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat.second, width, height, 0, textureFormat.first, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mShaderResource, 0);

	//glGenRenderbuffers(1, &mDepthStencil);
	//glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencil);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height); 
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthStencil);

	glCreateTextures(GL_TEXTURE_2D, 1, &mDepthStencil);
	glBindTexture(GL_TEXTURE_2D, mDepthStencil);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH32F_STENCIL8, width, height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthStencil, 0);
	
	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	mViewportTopLeftX = 0;
	mViewportTopLeftY = 0;
	mViewportWidth = width;
	mViewportHeight = height;
}

void RenderTarget::Terminate()
{
	//glDeleteRenderbuffers(1, &mDepthStencil);
	glDeleteTextures(1, &mDepthStencil);
	glDeleteTextures(1, &mShaderResource);
	glDeleteFramebuffers(1, &mRenderTarget);
}

void RenderTarget::BeginRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mRenderTarget);
	//glEnable(GL_DEPTH_TEST);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(mViewportTopLeftX, mViewportTopLeftY, mViewportWidth, mViewportHeight);
}

void RenderTarget::EndRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisable(GL_DEPTH_TEST);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GraphicsSystem::Get()->ResetViewport();
}

void RenderTarget::BindPS(uint32_t slot) const
{
	glBindTextureUnit(slot, mShaderResource);
}

void RenderTarget::UnbindPS(uint32_t slot) const
{
	glBindTextureUnit(slot, 0);
}

void RenderTarget::BindDepthPS(uint32_t slot) const
{
	glBindTextureUnit(slot, mDepthStencil);
}
#endif