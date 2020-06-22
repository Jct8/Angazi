#pragma once
#ifdef ENABLE_DIRECTX11

namespace Angazi::Graphics
{
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();

}//namespace Angazi::Graphics

#endif