#include "GameState.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::LightGray);

	//Basic
	mVertices.emplace_back(Vertex{ Vector3{  0.0f,  0.5f, 0.0f } , Colors::Gold });
	mVertices.emplace_back(Vertex{ Vector3{  0.5f, -0.5f, 0.0f } , Colors::Purple });
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f, -0.5f, 0.0f } , Colors::White });

	mMeshes["Basic"] = mVertices;
	mVertices.clear();

	//Triforce
	mVertices.emplace_back(Vertex{ Vector3{  0.0f, 1.0f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{  0.5f, 0.0f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f, 0.0f, 0.0f }, Colors::Red });

	mVertices.emplace_back(Vertex{ Vector3{ 0.5f, 0.0f, 0.0f }, Colors::Blue });
	mVertices.emplace_back(Vertex{ Vector3{ 1.0f, -1.0f, 0.0f }, Colors::Blue });
	mVertices.emplace_back(Vertex{ Vector3{ 0.0f, -1.0f, 0.0f }, Colors::Blue });

	mVertices.emplace_back(Vertex{ Vector3{ 0.0f, -1.0f, 0.0f }, Colors::Green });
	mVertices.emplace_back(Vertex{ Vector3{ -1.0f, -1.0f, 0.0f }, Colors::Green });
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f, 0.0f, 0.0f  }, Colors::Green });

	mMeshes["TriForce"] = mVertices;
	mVertices.clear();

	//Heart
	mVertices.emplace_back(Vertex{ Vector3{ -0.47f, 0.0f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ 0.47f, 0.0f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ 0.0f, -1.0f, 0.0f }, Colors::Red });

	//left side
	mVertices.emplace_back(Vertex{ Vector3{ -0.47f, 0.0f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ -0.45f, 0.2f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ -0.25f, 0.0f, 0.0f }, Colors::Red });

	mVertices.emplace_back(Vertex{ Vector3{ -0.45f, 0.2f, 0.0f  }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ -0.35f, 0.4f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ -0.25f, 0.0f, 0.0f }, Colors::Red });

	mVertices.emplace_back(Vertex{ Vector3{ -0.35f, 0.4f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ -0.25f, 0.45f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ -0.25f, 0.0f, 0.0f }, Colors::Red });

	mVertices.emplace_back(Vertex{ Vector3{ -0.25f, 0.45f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ -0.15f, 0.4f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ -0.25f, 0.0f, 0.0f }, Colors::Red });

	mVertices.emplace_back(Vertex{ Vector3{ -0.15f, 0.4f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ -0.05f, 0.2f, 0.0f  }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ -0.25f, 0.0f, 0.0f }, Colors::Red });

	mVertices.emplace_back(Vertex{ Vector3{ -0.05f, 0.2f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{  0.0f, 0.0f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ -0.25f, 0.0f, 0.0f }, Colors::Red });

	//Right side
	mVertices.emplace_back(Vertex{ Vector3{ 0.25f, 0.0f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ 0.45f, 0.2f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ 0.47f, 0.0f, 0.0f }, Colors::Red });

	mVertices.emplace_back(Vertex{ Vector3{ 0.25f, 0.0f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ 0.35f, 0.4f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ 0.45f, 0.2f, 0.0f  }, Colors::Red });

	mVertices.emplace_back(Vertex{ Vector3{ 0.25f, 0.0f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ 0.25f, 0.45f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ 0.35f, 0.4f, 0.0f }, Colors::Red });

	mVertices.emplace_back(Vertex{ Vector3{ 0.25f, 0.0f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ 0.15f, 0.4f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ 0.25f, 0.45f, 0.0f }, Colors::Red });

	mVertices.emplace_back(Vertex{ Vector3{ 0.25f, 0.0f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ 0.05f, 0.2f, 0.0f  }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ 0.15f, 0.4f, 0.0f }, Colors::Red });

	mVertices.emplace_back(Vertex{ Vector3{ 0.25f, 0.0f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ 0.0f, 0.0f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ 0.05f, 0.2f, 0.0f }, Colors::Red });

	/*mVertices.emplace_back(Vertex{ Vector3{ -0.25f, 0.5f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ 0.0f, 0.0f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f, 0.0f, 0.0f }, Colors::Red });

	mVertices.emplace_back(Vertex{ Vector3{ 0.25f, 0.5f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ 0.5f, 0.0f, 0.0f }, Colors::Red });
	mVertices.emplace_back(Vertex{ Vector3{ 0.0f, 0.0f, 0.0f }, Colors::Red });*/

	mMeshes["Heart"] = mVertices;
	mVertices.clear();

	//LeftSide
	mVertices.emplace_back(Vertex{ Vector3{ -0.02f, 0.3f, 0.0f }, Colors::Gold });
	mVertices.emplace_back(Vertex{ Vector3{ -0.42f, 0.6f, 0.0f }, Colors::Black });
	mVertices.emplace_back(Vertex{ Vector3{ -0.02f, 0.5f, 0.0f }, Colors::Gold });

	mVertices.emplace_back(Vertex{ Vector3{ -0.42f, 0.6f, 0.0f }, Colors::Black });
	mVertices.emplace_back(Vertex{ Vector3{ -0.32f, 0.65f, 0.0f }, Colors::Black });
	mVertices.emplace_back(Vertex{ Vector3{ -0.02f, 0.5f, 0.0f }, Colors::Gold });


	mVertices.emplace_back(Vertex{ Vector3{ -0.02f, 0.2f, 0.0f }, Colors::Gold });
	mVertices.emplace_back(Vertex{ Vector3{ -0.37f, 0.3f, 0.0f }, Colors::Black });
	mVertices.emplace_back(Vertex{ Vector3{ -0.42f, 0.5f, 0.0f }, Colors::Black });

	mVertices.emplace_back(Vertex{ Vector3{ -0.02f, 0.2f, 0.0f }, Colors::Gold });
	mVertices.emplace_back(Vertex{ Vector3{ -0.12f, 0.0f, 0.0f }, Colors::Gold });
	mVertices.emplace_back(Vertex{ Vector3{ -0.37f, 0.3f, 0.0f }, Colors::Black });


	mVertices.emplace_back(Vertex{ Vector3{ -0.02f, 0.1f, 0.0f }, Colors::Gold });
	mVertices.emplace_back(Vertex{ Vector3{ -0.02f, -0.9f, 0.0f }, Colors::Black });
	mVertices.emplace_back(Vertex{ Vector3{ -0.07f, -0.8f, 0.0f }, Colors::Black });

	mVertices.emplace_back(Vertex{ Vector3{ -0.07f, -0.8f, 0.0f }, Colors::Black });
	mVertices.emplace_back(Vertex{ Vector3{ -0.12f, -0.08f, 0.0f }, Colors::Black });
	mVertices.emplace_back(Vertex{ Vector3{ -0.02f, 0.1f, 0.0f }, Colors::Gold });

	//RightSide
	mVertices.emplace_back(Vertex{ Vector3{ 0.02f, 0.5f, 0.0f }, Colors::Gold });
	mVertices.emplace_back(Vertex{ Vector3{ 0.42f, 0.6f, 0.0f }, Colors::Black });
	mVertices.emplace_back(Vertex{ Vector3{ 0.02f, 0.3f, 0.0f }, Colors::Gold });

	mVertices.emplace_back(Vertex{ Vector3{ 0.02f, 0.5f, 0.0f }, Colors::Gold });
	mVertices.emplace_back(Vertex{ Vector3{ 0.32f, 0.65f, 0.0f }, Colors::Black });
	mVertices.emplace_back(Vertex{ Vector3{ 0.42f, 0.6f, 0.0f }, Colors::Black });


	mVertices.emplace_back(Vertex{ Vector3{ 0.42f, 0.5f, 0.0f }, Colors::Black });
	mVertices.emplace_back(Vertex{ Vector3{ 0.37f, 0.3f, 0.0f }, Colors::Black });
	mVertices.emplace_back(Vertex{ Vector3{ 0.02f, 0.2f, 0.0f }, Colors::Gold });

	mVertices.emplace_back(Vertex{ Vector3{ 0.37f, 0.3f, 0.0f }, Colors::Black });
	mVertices.emplace_back(Vertex{ Vector3{ 0.12f, 0.0f, 0.0f }, Colors::Gold });
	mVertices.emplace_back(Vertex{ Vector3{ 0.02f, 0.2f, 0.0f }, Colors::Gold });


	mVertices.emplace_back(Vertex{ Vector3{ 0.07f, -0.8f, 0.0f }, Colors::Black });
	mVertices.emplace_back(Vertex{ Vector3{ 0.02f, -0.9f, 0.0f }, Colors::Black });
	mVertices.emplace_back(Vertex{ Vector3{ 0.02f, 0.1f, 0.0f }, Colors::Gold });

	mVertices.emplace_back(Vertex{ Vector3{ 0.02f, 0.1f, 0.0f }, Colors::Gold });
	mVertices.emplace_back(Vertex{ Vector3{ 0.12f, -0.08f, 0.0f }, Colors::Black });
	mVertices.emplace_back(Vertex{ Vector3{ 0.07f, -0.8f, 0.0f }, Colors::Black });

	mMeshes["Kobe"] = mVertices;
	mVertices.clear();

	auto device = GraphicsSystem::Get()->GetDevice();

#pragma region CreateVertexBuffer
	//Create vertex buffer
	HRESULT hr;
	int i = 0;
	for (auto iter = mMeshes.begin(); iter != mMeshes.end(); ++iter)
	{
		D3D11_BUFFER_DESC bufferDesc{};
		bufferDesc.ByteWidth = iter->second.size() * sizeof(Vertex);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		//Initialization
		D3D11_SUBRESOURCE_DATA initData{};
		initData.pSysMem = iter->second.data();

		//hresult is an error code as a long
		mVertexBufferList.emplace_back();
		hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBufferList.back());
		ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");
	}
	mVertexBuffer = mVertexBufferList.front();
	mCurrentVertexCount = mMeshes["Basic"].size();
#pragma endregion

#pragma region CompileAndCreateVertexShader
	//Compile and create vertex shader
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr; // ID3DBlob - block of memory
	ID3DBlob* errorBlob = nullptr; // gets passed to vram
	std::filesystem::path shader = Angazi::Core::FilePath::GetAssetFilePath() + "Shaders/DoSomething.fx";
	hr = D3DCompileFromFile(
		shader.c_str(),
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
	vertexLayout.push_back({ "COLOR",0,
		DXGI_FORMAT_R32G32B32A32_FLOAT,0,
		D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0 });

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
		shader.c_str(),
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
	for (size_t i = 0; i < mVertexBufferList.size(); i++)
		SafeRelease(mVertexBufferList[i]);
	SafeRelease(mVertexShader);
	SafeRelease(mInputLayout);
	SafeRelease(mPixelShader);
}

void GameState::Update(float deltatTime)
{
	if (InputSystem::Get()->IsKeyPressed(Input::KeyCode::ONE))
	{
		mVertexBuffer = mVertexBufferList[0];
		mCurrentVertexCount = mMeshes["Basic"].size();
	}
	if (InputSystem::Get()->IsKeyPressed(Input::KeyCode::TWO))
	{
		mVertexBuffer = mVertexBufferList[1];
		mCurrentVertexCount = mMeshes["Heart"].size();
	}
	if (InputSystem::Get()->IsKeyPressed(Input::KeyCode::THREE))
	{
		mVertexBuffer = mVertexBufferList[2];
		mCurrentVertexCount = mMeshes["Kobe"].size();
	}
	if (InputSystem::Get()->IsKeyPressed(Input::KeyCode::FOUR))
	{
		mVertexBuffer = mVertexBufferList[3];
		mCurrentVertexCount = mMeshes["TriForce"].size();
	}
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

	context->Draw(mCurrentVertexCount, 0);
}



