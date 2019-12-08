#include "GameState.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::LightGray);

	mCamera.SetPosition({ 0.0f,0.0f,-5.0f });
	mCamera.SetDirection({ 0.0f,0.0f, 1.0f });

	//Front
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f,  0.5f, 0.0f } , Colors::Blue });
	mVertices.emplace_back(Vertex{ Vector3{  0.5f, -0.5f, 0.0f } , Colors::Blue });
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f, -0.5f, 0.0f } , Colors::Blue });

	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,  0.5f, 0.0f } , Colors::Blue });
	mVertices.emplace_back(Vertex{ Vector3{  0.5f,  0.5f, 0.0f } , Colors::SlateBlue });
	//mVertices.emplace_back(Vertex{ Vector3{  0.5f, -0.5f, 0.0f } , Colors::Blue });

	//Back
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f,  0.5f, 1.0f } , Colors::Blue });
	mVertices.emplace_back(Vertex{ Vector3{  0.5f, -0.5f, 1.0f } , Colors::Blue });
	mVertices.emplace_back(Vertex{ Vector3{ -0.5f, -0.5f, 1.0f } , Colors::Blue });
	mVertices.emplace_back(Vertex{ Vector3{  0.5f,  0.5f, 1.0f } , Colors::SlateBlue });

	mIndices.push_back(0);
	mIndices.push_back(1);
	mIndices.push_back(2);

	mIndices.push_back(0);
	mIndices.push_back(3);
	mIndices.push_back(1);

	mIndices.push_back();
	mIndices.push_back(1);
	mIndices.push_back(2);

	mIndices.push_back(0);
	mIndices.push_back(3);
	mIndices.push_back(1);

	////Back
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f, -0.5f, 1.0f } , Colors::Blue });
	//mVertices.emplace_back(Vertex{ Vector3{  0.5f, -0.5f, 1.0f } , Colors::Blue });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,  0.5f, 1.0f } , Colors::Blue });

	//mVertices.emplace_back(Vertex{ Vector3{  0.5f, -0.5f, 1.0f } , Colors::Blue });
	//mVertices.emplace_back(Vertex{ Vector3{  0.5f, 0.5f,  1.0f } , Colors::Blue });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,  0.5f, 1.0f } , Colors::Blue });

	////Right
	//mVertices.emplace_back(Vertex{ Vector3{  0.5f,  0.5f, 0.0f } , Colors::Gold });
	//mVertices.emplace_back(Vertex{ Vector3{  0.5f, -0.5f, 1.0f } , Colors::Gold });
	//mVertices.emplace_back(Vertex{ Vector3{  0.5f, -0.5f, 0.0f } , Colors::Gold });

	//mVertices.emplace_back(Vertex{ Vector3{  0.5f,  0.5f, 0.0f } , Colors::Gold });
	//mVertices.emplace_back(Vertex{ Vector3{  0.5f,  0.5f, 1.0f } , Colors::Gold });
	//mVertices.emplace_back(Vertex{ Vector3{  0.5f, -0.5f, 1.0f } , Colors::Gold });

	////Left
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f, -0.5f, 0.0f } , Colors::Gold });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f, -0.5f, 1.0f } , Colors::Gold });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,  0.5f, 0.0f } , Colors::Gold });

	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f, -0.5f, 1.0f } , Colors::Gold });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,  0.5f, 1.0f } , Colors::Gold });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,  0.5f, 0.0f } , Colors::Gold });

	////Top
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,  0.5f, 0.0f } , Colors::Green });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,  0.5f, 1.0f } , Colors::Green });
	//mVertices.emplace_back(Vertex{ Vector3{  0.5f,  0.5f, 0.0f } , Colors::Green });

	//mVertices.emplace_back(Vertex{ Vector3{  0.5f,  0.5f, 0.0f } , Colors::Green });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f,  0.5f, 1.0f } , Colors::Green });
	//mVertices.emplace_back(Vertex{ Vector3{  0.5f,  0.5f, 1.0f } , Colors::Green });

	////Bottom
	//mVertices.emplace_back(Vertex{ Vector3{  0.5f, -0.5f, 0.0f } , Colors::Green });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f, -0.5f, 1.0f } , Colors::Green });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f, -0.5f, 0.0f } , Colors::Green });

	//mVertices.emplace_back(Vertex{ Vector3{  0.5f, -0.5f, 1.0f } , Colors::Green });
	//mVertices.emplace_back(Vertex{ Vector3{ -0.5f, -0.5f, 1.0f } , Colors::Green });
	//mVertices.emplace_back(Vertex{ Vector3{  0.5f, -0.5f, 0.0f } , Colors::Green });

	auto device = GraphicsSystem::Get()->GetDevice();


#pragma region CreateConstantBuffer
	//Create constant buffer
	D3D11_BUFFER_DESC constantBufferDesc{};
	constantBufferDesc.ByteWidth = sizeof(Matrix4);
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	HRESULT hr = device->CreateBuffer(&constantBufferDesc, nullptr, &mConstantBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create constant buffer.");
#pragma endregion

#pragma region CreateVertexBuffer
	//Create vertex buffer

	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = mVertices.size() * sizeof(Vertex);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	//Initialization
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = mVertices.data();

	//hresult is an error code as a long
	hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer.");
#pragma endregion

#pragma region CreateIndexBuffer
	//Create index Buffer
	bufferDesc.ByteWidth = mIndices.size() * sizeof(uint32_t);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	initData.pSysMem = mIndices.data();

	hr = device->CreateBuffer(&bufferDesc, &initData, &mIndexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create index buffer.");

#pragma endregion

	//Compile and create vertex shader
	mVertexShader.Initialize();
	mPixelShader.Initialize();

}

void GameState::Terminate()
{
	mVertexShader.Terminate();
	mPixelShader.Terminate();

	SafeRelease(mConstantBuffer);
	SafeRelease(mIndexBuffer);
	SafeRelease(mVertexBuffer);

}

void GameState::Update(float deltaTime)
{
	const float kMoveSpeed = 10.0f;
	const float kTurnSpeed = 1.0f;

	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(kMoveSpeed*deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-kMoveSpeed * deltaTime);
	mCamera.Yaw(inputSystem->GetMouseMoveX() *kTurnSpeed*deltaTime);
	mCamera.Pitch(inputSystem->GetMouseMoveY() *kTurnSpeed*deltaTime);

	if (inputSystem->IsKeyDown(KeyCode::A))
		mRotation += deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::D))
		mRotation -= deltaTime;
}

void GameState::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();

	auto matWorld = Matrix4::RotationY(mRotation);
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();
	auto matWVP = Transpose(matWorld * matView * matProj);
	context->UpdateSubresource(mConstantBuffer, 0, nullptr, &matWVP, 0, 0);
	context->VSSetConstantBuffers(0, 1, &mConstantBuffer);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	mVertexShader.Bind();
	mPixelShader.Bind();
	//mMeshBuffer.Draw();

	//context->Draw(mVertices.size(), 0); <- this is for when we dont have an index buffer
	context->DrawIndexed((UINT)mIndices.size(), 0, 0);

}