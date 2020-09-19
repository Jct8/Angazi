#include "GameState.h"
#include "ImGui/Inc/imgui.h"
#include <DirectXTex/DirectXTex/DirectXTexP.h>
#include "GraphicsDX11/Src/D3DUtil.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

namespace
{
	ID3D11ShaderResourceView* hdrShaderResourceView;
	Math::Matrix4 cubeLookDir[] =
	{
		Matrix4::RotationQuaternion(Quaternion::RotationLookAt({ 1.0f,  0.0f,  0.0f }, { 0.0f, -1.0f,  0.0f })),
		Matrix4::RotationQuaternion(Quaternion::RotationLookAt({-1.0f,  0.0f,  0.0f }, { 0.0f, -1.0f,  0.0f })),
		Matrix4::RotationQuaternion(Quaternion::RotationLookAt({ 0.0f,  1.0f,  0.0f }, { 0.0f,  0.0f,  1.0f })).
		Matrix4::RotationQuaternion(Quaternion::RotationLookAt({ 0.0f, -1.0f,  0.0f }, { 0.0f,  0.0f, -1.0f })),
		Matrix4::RotationQuaternion(Quaternion::RotationLookAt({ 0.0f,  0.0f,  1.0f }, { 0.0f, -1.0f,  0.0f })),
		Matrix4::RotationQuaternion(Quaternion::RotationLookAt({ 0.0f,  0.0f, -1.0f }, { 0.0f, -1.0f,  0.0f })),
	};
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::LightBlue);

	mCamera.SetNearPlane(0.1f);
	mCamera.SetFarPlane(100.0f);
	//mCamera.SetPosition({ 1.20f,1.80f,-2.40f });
	//mCamera.SetDirection({ -0.321f,-0.284f, 0.784f });
	mCamera.SetPosition({ });
	mCamera.SetDirection({ 0.0f , 0.0f , 1.0f });

	mDirectionalLight.direction = Normalize({ 0.327f,-0.382f, 0.864f });
	mDirectionalLight.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mDirectionalLight.diffuse = { 0.75f,0.75f,0.75f ,1.0f };
	mDirectionalLight.specular = { 0.5f,0.5f,0.5f ,1.0f };

	mMaterial.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.diffuse = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.specular = { 0.5f,0.5f,0.5f ,1.0f };
	mMaterial.power = 80.0f;


	// Effects
	mGroundStandardEffect.Initialize("../../Assets/Shaders/Standard.fx");
	mGroundStandardEffect.SetDiffuseTexture("../../Assets/Images/Floor/Stone_Tiles_004_diffuse.jpg");
	mGroundStandardEffect.SetNormalTexture("../../Assets/Images/Floor/Stone_Tiles_004_normal.jpg");
	mGroundStandardEffect.SetAOTexture("../../Assets/Images/Floor/Stone_Tiles_004_ao.jpg");
	mGroundStandardEffect.SetDisplacementTexture("../../Assets/Images/Floor/Stone_Tiles_004_height.png");
	mGroundStandardEffect.UseShadow(true);
	mGroundStandardEffect.SetBumpMapWeight(6.0f);

	mGroundMeshBuffer.Initialize(MeshBuilder::CreatePlane(100.0f, 50, 50));

	DirectX::ScratchImage image;
	//std::array<ID3D11Resource*, 6> resourceArray;

	HRESULT hr = DirectX::LoadFromHDRFile(L"../../Assets/Images/HdrMaps/Shiodome_Stairs/10-Shiodome_Stairs_3k.hdr", nullptr, image);
	ASSERT(SUCCEEDED(hr), "[Texture] Failed to load texture");
	hr = CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), image.GetMetadata(), &hdrShaderResourceView);

	renderTarget.Initialize(512, 512, RenderTarget::Format::RGBA_F16);
	meshBuffer.Initialize(MeshBuilder::CreateInnerCubeP());
	vertexShader.Initialize("../../Assets/Shaders/Equirectangular.fx", VertexP::Format);
	pixelShader.Initialize("../../Assets/Shaders/Equirectangular.fx");
	tranformBuffer.Initialize();

	camera.SetNearPlane(0.1f);
	camera.SetFarPlane(10.0f);
	camera.SetPosition({ });
	camera.SetDirection({ 0.0f,0.0f ,1.0f });
	camera.SetFov(90.0f * Math::Constants::DegToRad);
	camera.SetAspectRatio(1.0f);

	mSkybox.CreateSkybox();
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);
	mHdrEffect.Initialize();
}

void GameState::Terminate()
{
	mHdrEffect.Terminate();
	mSampler.Terminate();

	mSkybox.Terminate();

	tranformBuffer.Terminate();
	meshBuffer.Terminate();
	pixelShader.Terminate();
	vertexShader.Terminate();
	renderTarget.Terminate();
	SafeRelease(hdrShaderResourceView);

	// Effects
	mGroundStandardEffect.Terminate();

}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 10.0f;
	const float kTurnSpeed = 10.0f * Constants::DegToRad;

	if (inputSystem->IsKeyDown(KeyCode::W))
		mCamera.Walk(kMoveSpeed*deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::S))
		mCamera.Walk(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(kMoveSpeed*deltaTime);
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() *kTurnSpeed*deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() *kTurnSpeed*deltaTime);
	}

}

void GameState::Render()
{
	mHdrEffect.BeginRender();
	DrawScene();
	mHdrEffect.EndRender();

	mHdrEffect.RenderHdrQuad();
}

void GameState::DebugUI()
{
	ImGui::ShowDemoWindow();

	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %.2f", Angazi::Core::TimeUtil::GetFramesPerSecond());
	/*ImGui::Image(
		mShadowEffect.GetRenderTarget()->GetShaderResourceView(),
		{ 150.0f,150.0f },
		{ 0.0f,0.0f },
		{ 1.0f,1.0f },
		{ 1.0f,1.0f ,1.0f,1.0f },
		{ 1.0f,1.0f ,1.0f,1.0f }
	);*/
	if (ImGui::CollapsingHeader("Light"))
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
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.x);
		ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.x);
		ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.x);
		ImGui::DragFloat("Power##Material", &mMaterial.power, 1.0f, 1.0f, 100.0f);
	}
	if (ImGui::CollapsingHeader("Settings"))
	{
		static bool normal = true;
		static bool specular = true;
		static bool aoMap = true;
		//static bool useShadow = mSettings.useShadow == 1;
		if (ImGui::Checkbox("Normal Map", &normal))
		{
			float normalMapWeight = normal ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Specular Map", &specular))
		{
			float specularMapWeight = specular ? 1.0f : 0.0f;
		}
		if (ImGui::Checkbox("Ambient occlusion", &aoMap))
		{
			float aoMapWeight = aoMap ? 1.0f : 0.0f;
		}
	}


	ImGui::End();
}

void GameState::DrawScene()
{
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	auto matWorld = Matrix4::Identity;
	auto matProj2 = camera.GetPerspectiveMatrix();
	//auto matView2 = camera.GetViewMatrix();
	

	mSampler.BindPS();

	for (int i = 0; i < 6; ++i)
	{
		auto matView2 = cubeLookDir[2];
		//renderTarget.BeginRender();
		pixelShader.Bind();
		vertexShader.Bind();
		GetContext()->PSSetShaderResources(0, 1, &hdrShaderResourceView);
		tranformBuffer.Set(Math::Transpose(matView2 *matProj2));
		tranformBuffer.BindVS(0);
		meshBuffer.Draw();
		//renderTarget.EndRender();
		//renderTarget.GetShaderResourceViewPointer()->GetResource(&resourceArray[i]);
	}

	// Ground
	matWorld = Matrix4::Translation({ 0.0f,-2.5f,0.0f });;
	mGroundStandardEffect.Begin();
	mGroundStandardEffect.SetMaterial(mMaterial);
	mGroundStandardEffect.SetDirectionalLight(mDirectionalLight);
	mGroundStandardEffect.SetViewPosition(mCamera.GetPosition());
	mGroundStandardEffect.SetWorldMatrix(matWorld);
	mGroundStandardEffect.SetWVPMatrix(matWorld, matView, matProj);
	mGroundStandardEffect.UpdateSettings();

	mGroundMeshBuffer.Draw();
	mGroundStandardEffect.End();

	SimpleDraw::AddGroundPlane(200,false,Colors::LightGray);

	SimpleDraw::Render(mCamera);
	mSkybox.Draw(mCamera);
}
