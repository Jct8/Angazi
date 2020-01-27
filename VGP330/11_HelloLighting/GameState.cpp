#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ 0.0f, 0.0f,-4.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });

	mMesh = MeshBuilder::CreateSphere(1.0f, 16, 16);
	mMeshBuffer.Initialize(mMesh);

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f,1.0f });
	mDirectionalLight.ambient = { 0.3f };
	mDirectionalLight.diffuse = { 0.7f };
	mDirectionalLight.specular = { 0.5f };

	mMaterial.ambient = { 0.3f };
	mMaterial.diffuse = { 0.7f };
	mMaterial.specular = { 0.5f };
	mMaterial.power = 80.0f;

	mGouraudShadingVertexShader.Initialize("../../Assets/Shaders/DoGouraudShading.fx", VertexPN::Format);
	mGouraudShadingPixelShader.Initialize("../../Assets/Shaders/DoGouraudShading.fx");

	mPhongShadingVertexShader.Initialize("../../Assets/Shaders/DoPhongShading.fx", Vertex::Format);
	mPhongShadingPixelShader.Initialize("../../Assets/Shaders/DoPhongShading.fx");

	mPointPhongShadingVertexShader.Initialize("../../Assets/Shaders/DoPhongPointShading.fx", Vertex::Format);
	mPointPhongShadingPixelShader.Initialize("../../Assets/Shaders/DoPhongPointShading.fx");

	mFlatShadingVertexShader.Initialize("../../Assets/Shaders/DoFlatShading.fx", VertexPN::Format);
	mFlatShadingPixelShader.Initialize("../../Assets/Shaders/DoFlatShading.fx");

	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Clamp);
	mTexture.Initialize("../../Assets/Images/white.jpg");
	mSpecularTexture.Initialize("../../Assets/Images/white.jpg");
}

void GameState::Terminate()
{
	mSpecularTexture.Terminate();
	mTexture.Terminate();
	mSampler.Terminate();

	mFlatShadingPixelShader.Terminate();
	mFlatShadingVertexShader.Terminate();
	mPointPhongShadingPixelShader.Terminate();
	mPointPhongShadingVertexShader.Terminate();
	mPhongShadingPixelShader.Terminate();
	mPhongShadingVertexShader.Terminate();
	mGouraudShadingPixelShader.Terminate();
	mGouraudShadingVertexShader.Terminate();

	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
	mMeshBuffer.Terminate();
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
	if (inputSystem->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(inputSystem->GetMouseMoveX() *kTurnSpeed*deltaTime);
		mCamera.Pitch(inputSystem->GetMouseMoveY() *kTurnSpeed*deltaTime);
	}

	if (inputSystem->IsKeyDown(KeyCode::A))
		mCamera.Strafe(-kMoveSpeed * deltaTime);
	if (inputSystem->IsKeyDown(KeyCode::D))
		mCamera.Strafe(kMoveSpeed*deltaTime);

}

void GameState::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();

	auto matTrans = Matrix4::Translation({ -1.0f,0.0f,0.0f });
	auto matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y);
	auto matWorld = matRot * matTrans;
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	TransformData transformData;
	mTransformBuffer.BindVS(0);

	mLightBuffer.Update(&mDirectionalLight);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.Update(&mMaterial);
	mMaterialBuffer.BindVS(2);
	mMaterialBuffer.BindPS(2);

	//ball 1 - gouroud
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView *matProj);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Update(&transformData);

	mGouraudShadingVertexShader.Bind();
	mGouraudShadingPixelShader.Bind();

	mMeshBuffer.Draw();

	//ball 2 - flat
	matTrans = Matrix4::Translation({ -3.0f,0.0f,0.0f });
	matWorld = matRot * matTrans;

	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView *matProj);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Update(&transformData);

	mFlatShadingVertexShader.Bind();
	mFlatShadingPixelShader.Bind();

	mMeshBuffer.Draw();

	//ball 3 - phong
	mSampler.BindVS();
	mSampler.BindPS();
	mTexture.BindPS(0);
	mTexture.BindVS(0);
	mSpecularTexture.BindPS(1);
	mSpecularTexture.BindVS(1);

	matTrans = Matrix4::Translation({ 1.5f,0.0f,0.0f });
	matWorld = matRot * matTrans;

	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView *matProj);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Update(&transformData);

	mPhongShadingVertexShader.Bind();
	mPhongShadingPixelShader.Bind();

	mMeshBuffer.Draw();

	//ball 4 - phong point

	matTrans = Matrix4::Translation({ 3.75f,0.0f,0.0f });
	matWorld = matRot * matTrans;

	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView *matProj);
	transformData.viewPosition = mCamera.GetPosition();
	mTransformBuffer.Update(&transformData);

	mPointPhongShadingVertexShader.Bind();
	mPointPhongShadingPixelShader.Bind();

	mMeshBuffer.Draw();

}

void GameState::DebugUI()
{
	ImGui::Begin("Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool directionChanged = false;
		directionChanged |= ImGui::DragFloat("Direction X##Light", &mDirectionalLight.direction.x, 0.01f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Y##Light", &mDirectionalLight.direction.y, 0.01f, -1.0f, 1.0f);
		directionChanged |= ImGui::DragFloat("Direction Z##Light", &mDirectionalLight.direction.z, 0.01f, -1.0f, 1.0f);
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
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Rotation##Transform", &mRotation.x, 0.01f);
	}
	ImGui::End();
}