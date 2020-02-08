#pragma once

namespace Angazi::Graphics {

class Blending
{
public:
	void Initialize();
	void Terminate();

	void Bind()  const;
	void UnBind()  const;

private:
	ID3D11BlendState* mD3dBlendState = nullptr;

};

} // namespace Angazi::Graphics