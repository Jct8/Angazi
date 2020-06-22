#include "Precompiled.h"

#ifdef ENABLE_DIRECTX11

#include "VertexShader.h"

#include "Graphics/Inc/VertexTypes.h"
#include "D3DUtil.h"

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	auto GetVertexLayout(uint32_t vertexFormat)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout; // rvo - copy elision

		if (vertexFormat & VE_Position)
			vertexLayout.push_back({ "POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT,0,
				D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 });

		if (vertexFormat & VE_Normal)
			vertexLayout.push_back({ "NORMAL",0, DXGI_FORMAT_R32G32B32_FLOAT,0,
				D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 });

		if (vertexFormat & VE_Tangent)
			vertexLayout.push_back({ "TANGENT",0, DXGI_FORMAT_R32G32B32_FLOAT,0,
				D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 });

		if (vertexFormat & VE_Color)
			vertexLayout.push_back({ "COLOR",0, DXGI_FORMAT_R32G32B32A32_FLOAT,0,
				D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 });

		if (vertexFormat & VE_TexCoord)
			vertexLayout.push_back({ "TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT,0,
				D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 });

		if (vertexFormat & VE_BlendIndex)
			vertexLayout.push_back({ "BLENDINDICES",0, DXGI_FORMAT_R32G32B32A32_SINT,0,
				D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 });

		if (vertexFormat & VE_BlendWeight)
			vertexLayout.push_back({ "BLENDWEIGHT",0, DXGI_FORMAT_R32G32B32A32_FLOAT,0,
				D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 });

		return vertexLayout;
	}
}

VertexShader::~VertexShader()
{
	ASSERT(mVertexShader == nullptr, "[mVertexShader] Terminate() must be called to clean up!");
	ASSERT(mInputLayout == nullptr, "[mVertexShader] Terminate() must be called to clean up!");
}

void VertexShader::Initialize(const std::filesystem::path & filePath, uint32_t vertextFormat, const char* shaderName)
{
	//Compile and create vertex shader
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr; // ID3DBlob - block of memory
	ID3DBlob* errorBlob = nullptr; // gets passed to vram
	HRESULT hr = D3DCompileFromFile(
		filePath.wstring().c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		shaderName, "vs_5_0",
		shaderFlags, 0, &shaderBlob, &errorBlob);
	if (errorBlob && errorBlob->GetBufferPointer())
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	ASSERT(SUCCEEDED(hr), "Failed to compile vertex shader.");

	auto device = GetDevice();
	hr = device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader.");

	auto vertexLayout = GetVertexLayout(vertextFormat);

	hr = device->CreateInputLayout(
		vertexLayout.data(),
		static_cast<UINT>(vertexLayout.size()),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout);
	ASSERT(SUCCEEDED(hr), "Failed to create input layout.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void VertexShader::Terminate()
{
	SafeRelease(mInputLayout);
	SafeRelease(mVertexShader);
}

void VertexShader::Bind()
{
	auto context = GetContext();
	context->IASetInputLayout(mInputLayout);
	context->VSSetShader(mVertexShader, nullptr, 0);
}

#endif