#include "Precompiled.h"
#include "BlendStateGL.h"

#ifdef ENABLE_OPENGL

using namespace Angazi::Graphics;

namespace
{
	std::pair<int, int> GetBlend(BlendState::Mode mode)
	{
		switch (mode)
		{
		case Angazi::Graphics::BlendState::Mode::Opaque:
			return { GL_ONE , GL_ZERO };
		case Angazi::Graphics::BlendState::Mode::AlphaBlend:
			return { GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA };
		case Angazi::Graphics::BlendState::Mode::AlpahPremultiplied:
			return { GL_ONE , GL_ONE_MINUS_SRC_ALPHA };
		case Angazi::Graphics::BlendState::Mode::Additive:
			return { GL_SRC_ALPHA , GL_ONE };
			
		default:
			break;
		}
		return { GL_ONE , GL_ZERO };
	}
}


void BlendState::ClearState()
{
	glDisable(GL_BLEND);
}

BlendState::~BlendState()
{
}

void Angazi::Graphics::BlendState::Initialize(Mode mode)
{
	glEnable(GL_BLEND);
	mBlendState = mode;
}

void BlendState::Terminate()
{
	glDisable(GL_BLEND);
}

void BlendState::Bind() const
{
	glEnable(GL_BLEND);
	auto blend = GetBlend(mBlendState);
	glBlendFunc(blend.first, blend.second);
}

#endif