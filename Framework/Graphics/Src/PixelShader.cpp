#include "Precompiled.h"
#include "PixelShader.h"

#include "D3DUtil.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void Angazi::Graphics::PixelShader::Initialize(const std::filesystem::path& filePath)
{
	//CompileAndCreatePixelShader
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr; // ID3DBlob - block of memory
	ID3DBlob* errorBlob = nullptr; // gets passed to vram

	HRESULT hr = D3DCompileFromFile(
		filePath.wstring().c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderFlags, 0, &shaderBlob, &errorBlob);
	if (errorBlob && errorBlob->GetBufferPointer())
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	ASSERT(SUCCEEDED(hr), "Failed to compile pixel buffer.");

	auto device = GetDevice();
	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);


}

void Angazi::Graphics::PixelShader::Terminate()
{
	SafeRelease(mPixelShader);

}

void Angazi::Graphics::PixelShader::Bind()
{
	auto context = GetContext();
	context->PSSetShader(mPixelShader, nullptr, 0);
}
