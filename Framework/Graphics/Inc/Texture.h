#pragma once

namespace Angazi::Graphics {

class Texture
{
public:
	void Initialize(const std::filesystem::path& fileName);
	void Terminate();

	void BindVS(uint32_t slot = 0) const;
	void BindPS(uint32_t slot = 0) const;

private:
	ID3D11ShaderResourceView* mShaderResourceView = nullptr;
};

} // namespace Angazi::Graphics