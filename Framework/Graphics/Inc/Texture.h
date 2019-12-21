#pragma once

namespace Angazi::Graphics {

class Texture
{
public:
	void Initialize(const std::filesystem::path& fileName);
	void Terminate();

	void Bind() const;

private:
	ID3D11ShaderResourceView* mShaderResourceView = nullptr;
};

} // namespace Angazi::Graphics