#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

void GameState::Initialize()
{

	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	/*D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	rasterDesc.FrontCounterClockwise = true;
	rasterDesc.DepthBias = 0;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.ScissorEnable = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.AntialiasedLineEnable = false;

	HRESULT hr = GraphicsSystem::Get()->GetDevice()->CreateRasterizerState(&rasterDesc, &mRasterState);
	ASSERT(SUCCEEDED(hr), "[RasterState] Failed to create raster state.");

	GraphicsSystem::Get()->GetContext()->RSSetState(mRasterState);*/

	mCamera.SetPosition({ 0.0f,3.0f,-5.0f });
	mCamera.SetDirection({ 0.0f,0.0f, 1.0f });

	////////////
	mMesh = MeshBuilder::CreatePlane(200.0f, 20, 20);
	mMeshBuffer.Initialize(mMesh);

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f,1.0f });
	mDirectionalLight.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mDirectionalLight.diffuse = { 0.75f,0.75f,0.75f ,1.0f };
	mDirectionalLight.specular = { 0.5f,0.5f,0.5f ,1.0f };

	mMaterial.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.diffuse = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.specular = { 0.5f,0.5f,0.5f ,1.0f };
	mMaterial.power = 80.0f;

	mVertexShader.Initialize("../../Assets/Shaders/Water.fx", Vertex::Format);
	mPixelShader.Initialize("../../Assets/Shaders/Water.fx");

	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
	mTexture.Initialize("../../Assets/Images/waterjpg.jpg");
	mGroundTexture.Initialize("../../Assets/Images/grass.jpg");
	mSpecularTexture.Initialize("../../Assets/Images/earth_spec.jpg");
	mDisplacementTexture.Initialize("../../Assets/Images/dudv.png");
	mNormalMap.Initialize("../../Assets/Images/earth_normal.jpg");

	mBlendState.Initialize(BlendState::Mode::Additive);

	//Quad
	auto graphicsSystem = GraphicsSystem::Get();
	mRenderTarget.Initialize(graphicsSystem->GetBackBufferWidth(), graphicsSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U8);
	mScreenQuad = MeshBuilder::CreateNDCQuad();
	mScreenQuadBuffer.Initialize(mScreenQuad);

	mPostProcessingVertexShader.Initialize("../../Assets/Shaders/PostProcessing.fx", VertexPX::Format);
	mPostProcessingPixelShader.Initialize("../../Assets/Shaders/PostProcessing.fx","PSNoProcessing");

	mRefractionRenderTarget.Initialize(graphicsSystem->GetBackBufferWidth(), graphicsSystem->GetBackBufferHeight(), RenderTarget::Format::RGBA_U32);
	mRefractionVertexShader.Initialize("../../Assets/Shaders/Standard.fx", Vertex::Format);
	mRefractionPixelShader.Initialize("../../Assets/Shaders/Standard.fx");
	mRefractionConstantBuffer.Initialize();

	mShadowConstantBuffer.Initialize();

	//Clipping
	mClippingConstantBuffer.Initialize();
}

void GameState::Terminate()
{
	SafeRelease(mRasterState);
	mClippingConstantBuffer.Terminate();

	mShadowConstantBuffer.Terminate();
	mRefractionRenderTarget.Terminate();
	mRefractionVertexShader.Terminate();
	mRefractionPixelShader.Terminate();
	mRefractionConstantBuffer.Terminate();

	mPostProcessingPixelShader.Terminate();
	mPostProcessingVertexShader.Terminate();
	mScreenQuadBuffer.Terminate();
	mRenderTarget.Terminate();
	//
	mBlendState.Terminate();
	mNormalMap.Terminate();
	mDisplacementTexture.Terminate();
	mGroundTexture.Terminate();
	mTexture.Terminate();
	mSpecularTexture.Terminate();
	mSampler.Terminate();

	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
	mMeshBuffer.Terminate();
}

void GameState::Update(float deltaTime)
{
	const float kMoveSpeed = 5.0f;
	const float kTurnSpeed = 1.0f;

	auto inputSystem = InputSystem::Get();
	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(kMoveSpeed*deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() *kTurnSpeed*deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() *kTurnSpeed*deltaTime);
	}

	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(kMoveSpeed*deltaTime);
	mSettings.movement += mSettings.movementSpeed * deltaTime;
	if (mSettings.movement >= 1.0f)
		mSettings.movement -= 1.0f;

}

void GameState::Render()
{
	mRefractionRenderTarget.BeginRender();
	DrawRefraction();
	mRefractionRenderTarget.EndRender();

	mRenderTarget.BeginRender();
	DrawScene();
	mRenderTarget.EndRender();

	mRenderTarget.BindPS(0);
	PostProcess();
	mRenderTarget.UnbindPS(0);
}

void GameState::DebugUI()
{
	ImGui::Begin("Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Image(
			mRefractionRenderTarget.GetShaderResourceView(),
			{ 150.0f,150.0f },
			{ 0.0f,0.0f },
			{ 1.0f,1.0f },
			{ 1.0f,1.0f ,1.0f,1.0f },
			{ 1.0f,1.0f ,1.0f,1.0f }
		);
	}
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat("Direction X##Light", &mDirectionalLight.direction.x, 0.01f, -2.0f, 2.0f);
		directionChanged |= ImGui::DragFloat("Direction Y##Light", &mDirectionalLight.direction.y, 0.01f, -2.0f, 2.0f);
		directionChanged |= ImGui::DragFloat("Direction Z##Light", &mDirectionalLight.direction.z, 0.01f, -2.0f, 2.0f);
		if (directionChanged)
		{
			mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.x);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.x);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.x);
	}
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.x);
		ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.x);
		ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.x);
		ImGui::DragFloat("Power##Material", &mMaterial.power, 1.0f, 1.0f, 100.0f);
	}
	if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		static bool normal = true;
		static bool specular = true;
		static bool aoMap = true;
		static bool useShadow = mSettings.useShadow == 1;
		ImGui::SliderFloat("Displacement", &mSettings.bumpMapWeight, 0.0f, 1.0f);
		if (ImGui::Checkbox("Normal Map", &normal))
		{
			mSettings.normalMapWeight = normal ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Specular Map", &specular))
		{
			mSettings.specularMapWeight = specular ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Ambient occlusion", &aoMap))
		{
			mSettings.aoMapWeight = aoMap ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Use Shadow", &useShadow))
		{
			mSettings.useShadow = useShadow ? 1 : 0;
		}
		ImGui::SliderFloat("Brightness", &mSettings.brightness, 1.0f, 10.f);
		ImGui::SliderFloat("Movement Speed", &mSettings.movementSpeed, 0.0001f, 0.1f);
	}
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Rotation##Transform", &mRotation.x, 0.01f);
	}
	ImGui::End();
}

void GameState::DrawRefraction()
{
	//Water
	auto matTrans = Matrix4::Translation({ -1.0f,0.0f,0.0f });
	auto matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z);
	auto matWorld = matRot * matTrans;
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	Clipping clipping;
	clipping.plane = { 0.0f,1.0f,0.0f,0.0f };
	clipping.distance = 20;
	mClippingConstantBuffer.Update(&clipping);
	mClippingConstantBuffer.BindVS(4);

	//water
	mSampler.BindVS();
	mSampler.BindPS();

	mTexture.BindPS(0);

	//mSpecularTexture.BindPS(1);
	mDisplacementTexture.BindVS(2);
	mDisplacementTexture.BindPS(2);
	//mNormalMap.BindPS(3);

	TransformData transformData;
	mTransformBuffer.BindVS(0);

	mLightBuffer.Update(&mDirectionalLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(&mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.Update(&mSettings);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView *matProj);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Update(&transformData);

	mVertexShader.Bind();
	mPixelShader.Bind();


	//mBlendState.Bind();
	//mMeshBuffer.Draw();
	//mBlendState.ClearState();

	//ground
	mGroundSettings.bumpMapWeight = 0.0f;
	mGroundSettings.brightness = 1.0f;
	mSettingsBuffer.Update(&mGroundSettings);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mRefractionPixelShader.Bind();
	mRefractionVertexShader.Bind();
	matTrans = Matrix4::Translation({ -1.0f,-2.0f,0.0f });
	matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z);
	matWorld = matRot * matTrans;

	mGroundTexture.BindPS(0);

	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView *matProj);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Update(&transformData);

	mMeshBuffer.Draw();
}

void GameState::DrawScene()
{
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	Clipping clipping;
	clipping.plane = { 0.0f,1.0f,0.0f,0.0f };
	clipping.distance = 20;
	mClippingConstantBuffer.Update(&clipping);
	mClippingConstantBuffer.BindVS(4);

	mSampler.BindVS();
	mSampler.BindPS();

	TransformData transformData;
	mTransformBuffer.BindVS(0);

	mLightBuffer.Update(&mDirectionalLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(&mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.Update(&mSettings);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	//Water
	auto matTrans = Matrix4::Translation({ -1.0f,0.0f,0.0f });
	auto matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z);
	auto matWorld = matRot * matTrans;

	//mTexture.BindPS(0);
	mRefractionRenderTarget.BindPS(0);
	//mSpecularTexture.BindPS(1);
	mDisplacementTexture.BindVS(2);
	mDisplacementTexture.BindPS(2);
	//mNormalMap.BindPS(3);

	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView *matProj);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Update(&transformData);

	mVertexShader.Bind();
	mPixelShader.Bind();

	//mBlendState.Bind();
	mMeshBuffer.Draw();
	mBlendState.ClearState();

	//ground
	mGroundSettings.bumpMapWeight = 0.0f;
	mGroundSettings.brightness = 1.0f;
	mSettingsBuffer.Update(&mGroundSettings);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mRefractionPixelShader.Bind();
	mRefractionVertexShader.Bind();
	matTrans = Matrix4::Translation({ -1.0f,-2.0f,0.0f });
	matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z);
	matWorld = matRot * matTrans;

	mGroundTexture.BindPS(0);

	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView *matProj);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Update(&transformData);


	mMeshBuffer.Draw();
}

void GameState::PostProcess()
{
	mPostProcessingPixelShader.Bind();
	mPostProcessingVertexShader.Bind();
	mSampler.BindPS();
	mScreenQuadBuffer.Draw();
}
