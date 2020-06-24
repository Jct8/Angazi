#include "Precompiled.h"
#include "SamplerGL.h"

#ifdef ENABLE_OPENGL

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	uint32_t GetFilter(Sampler::Filter filter)
	{
		switch (filter)
		{
		case Sampler::Filter::Point:
			return GL_NEAREST_MIPMAP_NEAREST;
		case Sampler::Filter::Linear:
			return GL_LINEAR_MIPMAP_LINEAR;
		case Sampler::Filter::Anisotropic:
			return GL_TEXTURE_MAX_ANISOTROPY_EXT;
		}
		return GL_NEAREST_MIPMAP_NEAREST;
	}

	uint32_t GetAddressMode(Sampler::AddressMode mode)
	{
		switch (mode)
		{
		case Sampler::AddressMode::Border:
			return GL_CLAMP_TO_BORDER;
		case Sampler::AddressMode::Clamp:
			return GL_CLAMP_TO_EDGE;
		case Sampler::AddressMode::Mirror:
			return GL_MIRRORED_REPEAT;
		case Sampler::AddressMode::Wrap:
			return GL_REPEAT;
		}
		return GL_CLAMP_TO_BORDER;
	}
}

Sampler::~Sampler()
{
	ASSERT(!glIsSampler(mSampler), "[SamplerGL] Terminate() must be called to clean up!");
}

void Sampler::Initialize(Filter filter, AddressMode addressMode)
{
	auto GLfilter = GetFilter(filter);
	auto GLaddressMode = GetAddressMode(addressMode);

	glGenSamplers(1, &mSampler);
	glSamplerParameteri(mSampler, GL_TEXTURE_MIN_FILTER, GLfilter);
	glSamplerParameteri(mSampler, GL_TEXTURE_MAG_FILTER, GLfilter);

	glSamplerParameteri(mSampler, GL_TEXTURE_WRAP_S, GLaddressMode);
	glSamplerParameteri(mSampler, GL_TEXTURE_WRAP_T, GLaddressMode);
}

void Sampler::Terminate()
{
	glBindSampler(GL_TEXTURE_2D, 0);
	glDeleteSamplers(1, &mSampler);
}

void Sampler::BindVS(uint32_t slot) const
{
	glBindSampler(GL_TEXTURE_2D, mSampler);
}

void Sampler::BindPS(uint32_t slot) const
{
	glBindSampler(GL_TEXTURE_2D, mSampler);
}

#endif