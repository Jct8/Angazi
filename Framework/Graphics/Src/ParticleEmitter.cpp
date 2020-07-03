#include "Precompiled.h"
#include "ParticleEmitter.h"

#include "VertexTypes.h"
#include "Camera.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void ParticleEmitter::Initialize(const std::filesystem::path& imagePath, uint32_t maxParticles)
{
	mMeshBuffer.Initialize<VertexPCX>(nullptr, maxParticles * 6, true);
	mVertexShader.Initialize("../../Assets/Shaders/Particles.fx", VertexPCX::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Particles.fx");
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Clamp);
	mTexture.Initialize(imagePath);
	mBlendState.Initialize(BlendState::Mode::Additive);
	mConstantBuffer.Initialize(sizeof(Math::Matrix4));
	mDepthStencilState.Initialize(true, false);
	mMaxParticles = maxParticles;
}

void ParticleEmitter::Terminate()
{
	mDepthStencilState.Terminate();
	mConstantBuffer.Terminate();
	mBlendState.Terminate();
	mTexture.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();
}

void ParticleEmitter::Start(float emitRate)
{
	mEmitRate = emitRate;
}

void ParticleEmitter::Stop()
{
	mEmitRate = 0.0f;
}

void ParticleEmitter::Update(float deltaTime)
{
	// Spawn new particles
	mEmitCount += mEmitRate * deltaTime;
	int count = static_cast<int>(mEmitCount);
	for (int i = 0; i < count; ++i)
	{
		Particle p;
		p.position = mPosition;
		p.position.x += Math::RandomFloat(-0.05f, 0.05f);
		p.position.y += Math::RandomFloat(-0.05f, 0.05f);
		p.position.z += Math::RandomFloat(-0.05f, 0.05f);
		p.velocity = Math::Vector3::Zero;
		p.acceleration = Math::Vector3::Zero;
		p.startColor = mStartColor;
		p.endColor = mEndColor;
		p.startSize = mStartSize;
		p.endSize = mEndSize;
		p.lifetime = p.maxLifetime = Math::RandomFloat(0.0f, mMaxLifetime);
		p.textureIndex = Math::RandomInt(0,4);
		mParticles.emplace_back(p);
	}
	mEmitCount -= count;

	// Update and prune existing particles
	for (auto&p : mParticles)
		p.lifetime -= deltaTime;

	// Erase-remove idiom
	auto isDead = [](auto& p) {return p.lifetime <= 0.0f; };
	mParticles.erase(std::remove_if(mParticles.begin(), mParticles.end(), isDead), mParticles.end());
}

void ParticleEmitter::Draw(const Camera & camera)
{
	auto matView = camera.GetViewMatrix();
	auto matProj = camera.GetPerspectiveMatrix();
	auto transform = Math::Transpose(matView * matProj);
	mConstantBuffer.Update(&transform);
	mConstantBuffer.BindVS(0);

	mVertexShader.Bind();
	mPixelShader.Bind();

	std::vector<VertexPCX> vertices;
	Math::Vector2 uv[] = { {0.0f, 0.0f}, {0.5f, 0.0f}, {0.0f, 0.5f}, {0.5f, 0.5f} };

	Math::Vector3 cameraDir = camera.GetDirection();
	std::sort(mParticles.begin(), mParticles.end(), [&cameraDir](auto& a, auto& b)
	{
		auto s = Dot(a.position, cameraDir);
		auto t = Dot(b.position, cameraDir);
		return s > t;
	});

	Math::Vector3 cameraPos = camera.GetPosition();
	Math::Vector3 up = Math::GetUp(matView);
	for (size_t i = 0; i < size_t(mMaxParticles) && i < mParticles.size(); ++i)
	{
		auto &p = mParticles[i];
		auto dirToCam = Math::Normalize(cameraPos - p.position);
		auto side = Cross(up, dirToCam);
		VertexPCX tl, tr, bl, br;
		float t = 1.0f - p.lifetime / p.maxLifetime;
		float s = Math::Lerp(p.startSize, p.endSize, t);
		Color c = Math::Lerp(p.startColor, p.endColor, t);
		tl.position = p.position + (side* s) + (up*s);
		tr.position = p.position - (side* s) + (up*s);
		bl.position = p.position + (side* s) - (up*s);
		br.position = p.position - (side* s) - (up*s);
		tl.color = c;
		tr.color = c;
		bl.color = c;
		br.color = c;
		tl.uv = uv[p.textureIndex];
		tr.uv = tl.uv + Math::Vector2{ 0.5f,0.0f };
		bl.uv = tl.uv + Math::Vector2{ 0.0f,0.5f };
		br.uv = tl.uv + Math::Vector2{ 0.5f,0.5f };
		vertices.push_back(tl);
		vertices.push_back(tr);
		vertices.push_back(bl);
		vertices.push_back(tr);
		vertices.push_back(br);
		vertices.push_back(bl);
	}
	mMeshBuffer.Update(vertices.data(), static_cast<uint32_t>(vertices.size()));
	mMeshBuffer.SetTopology(MeshBuffer::Topology::Triangles);

	mSampler.BindPS(0);
	mTexture.BindPS(0);

	mBlendState.Bind();
	mDepthStencilState.Set();
	mMeshBuffer.Draw();
	mDepthStencilState.Clear();
	mBlendState.ClearState();
}

void ParticleEmitter::SetPosition(const Math::Vector3 & pos)
{
	mPosition = pos;
}

void ParticleEmitter::SetDirection(const Math::Vector3 & dir, float span)
{
	mDirection = dir;
	mDirectionSpan = span;
}
