#include "GameState.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::LightGray);
	mVertices.emplace_back(Vertex{Vector3{  0.0f,  0.5f, 0.0f }});
	mVertices.emplace_back(Vertex{Vector3{  0.5f, -0.5f, 0.0f }});
	mVertices.emplace_back(Vertex{Vector3{ -0.5f, -0.5f, 0.0f }});

	auto device = GraphicsSystem::Get()->GetDevice();

#pragma region CreateVertexBuffer
	//Create vertex buffer
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = 3 * sizeof(Vertex);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	//Initialization
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = mVertices.data();

	//hresult is an error code as a long
	HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");
#pragma endregion

#pragma region CompileAndCreateVertexShader
	//Compile and create vertex shader
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr; // ID3DBlob- block of memory
	ID3DBlob* errorBlob = nullptr; // gets passed to vram
	hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoNothing.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0",
		shaderFlags, 0, &shaderBlob, &errorBlob);
	if (errorBlob && errorBlob->GetBufferPointer())
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	ASSERT(SUCCEEDED(hr), "Failed to compile vertex buffer.");

	hr = device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader.");

	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	vertexLayout.push_back({ "POSITION",0,
		DXGI_FORMAT_R32G32B32_FLOAT,0,
		D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 });
	/*vertexLayout.push_back({ "COLOR",0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,0,
		D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 });*/

	hr = device->CreateInputLayout(
		vertexLayout.data(),
		(UINT)vertexLayout.size(),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout);
	ASSERT(SUCCEEDED(hr), "Failed to create input layout.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);

#pragma endregion

#pragma region CompileAndCreatePixelShader
	hr = D3DCompileFromFile(
		L"../../Assets/Shaders/DoNothing.fx",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderFlags, 0, &shaderBlob, &errorBlob);
	if (errorBlob && errorBlob->GetBufferPointer())
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	ASSERT(SUCCEEDED(hr), "Failed to compile pixel buffer.");

	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader.");

	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
#pragma endregion



}

void GameState::Terminate()
{
	SafeRelease(mVertexBuffer);
	SafeRelease(mVertexShader);
	SafeRelease(mInputLayout);
	SafeRelease(mPixelShader);
}

void GameState::Update(float deltatTime)
{

}

void GameState::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();

	context->IASetInputLayout(mInputLayout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	context->VSSetShader(mVertexShader, nullptr, 0);
	context->PSSetShader(mPixelShader, nullptr, 0);

	context->Draw(mVertices.size(), 0);
}



