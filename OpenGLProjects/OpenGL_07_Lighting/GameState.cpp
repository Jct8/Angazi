#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::GraphicsGL;
using namespace Angazi::Input;
using namespace Angazi::Math;

void GameState::Initialize()
{
	GraphicsSystemGL::Get()->SetClearColor(Colors::LightGray);

	mCamera.SetPosition({ 0.0f, 0.0f,-2.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });

	mMesh = MeshBuilder::CreateSphere(1.0f,30,30);
	mMeshBuffer.Initialize(mMesh,VertexPN::Format);

	mDirectionalLight.direction = Normalize({ 1.0f, -1.0f,1.0f });
	mDirectionalLight.ambient = { 0.3f };
	mDirectionalLight.diffuse = { 0.7f };
	mDirectionalLight.specular = { 0.5f };

	mMaterial.ambient = { 0.3f };
	mMaterial.diffuse = { 0.7f };
	mMaterial.specular = { 0.5f };
	mMaterial.power = 1.0f;

	mGouraudShadingShader.Initialize("../../Assets/GLShaders/GLGouraudShading.glsl");
	mPhongShadingShader.Initialize("../../Assets/GLShaders/GLPhongShading.glsl");
}

void GameState::Terminate()
{
	mPhongShadingShader.Terminate();
	mGouraudShadingShader.Terminate();
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

	mGouraudShadingShader.SetUniform4f("material.ambient", mMaterial.ambient);
	mGouraudShadingShader.SetUniform4f("material.diffuse", mMaterial.diffuse);
	mGouraudShadingShader.SetUniform4f("material.specular",mMaterial.specular);
	mGouraudShadingShader.SetUniform1f("material.power",  mMaterial.power);

	mGouraudShadingShader.SetUniform4f("light.ambient", mDirectionalLight.ambient);
	mGouraudShadingShader.SetUniform4f("light.diffuse", mDirectionalLight.diffuse);
	mGouraudShadingShader.SetUniform4f("light.specular",mDirectionalLight.specular);
	mGouraudShadingShader.SetUniform3f("light.direction", mDirectionalLight.direction);

	mPhongShadingShader.SetUniform4f("material.ambient", mMaterial.ambient);
	mPhongShadingShader.SetUniform4f("material.diffuse", mMaterial.diffuse);
	mPhongShadingShader.SetUniform4f("material.specular", mMaterial.specular);
	mPhongShadingShader.SetUniform1f("material.power", mMaterial.power);

	mPhongShadingShader.SetUniform4f("light.ambient", mDirectionalLight.ambient);
	mPhongShadingShader.SetUniform4f("light.diffuse", mDirectionalLight.diffuse);
	mPhongShadingShader.SetUniform4f("light.specular", mDirectionalLight.specular);
	mPhongShadingShader.SetUniform3f("light.direction", mDirectionalLight.direction);

	//ball 1
	auto matTrans = Matrix4::Translation({ 0.0f });
	auto matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y);
	auto matWorld = matRot * matTrans;
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	TransformData transformData;
	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView *matProj);
	transformData.viewPosition = mCamera.GetPosition();

	mGouraudShadingShader.SetUniformMat4f("transform.WVP", transformData.wvp);
	mGouraudShadingShader.SetUniformMat4f("transform.World", transformData.world);
	mGouraudShadingShader.SetUniform3f("transform.ViewPosition", transformData.viewPosition);

	//mGouraudShadingShader.Bind();

	//mMeshBuffer.Draw();

	//ball 2
	matTrans = Matrix4::Translation({ 3.0f,0.0f,0.0f });
	matWorld = matRot * matTrans;

	transformData.world = Transpose(matWorld);
	transformData.wvp = Transpose(matWorld * matView *matProj);
	transformData.viewPosition = mCamera.GetPosition();

	mPhongShadingShader.SetUniformMat4f("transform.WVP", transformData.wvp);
	mPhongShadingShader.SetUniformMat4f("transform.World", transformData.world);
	mPhongShadingShader.SetUniform3f("transform.ViewPosition", transformData.viewPosition);

	mPhongShadingShader.Bind();

	mMeshBuffer.Draw();

}

void GameState::DebugUI()
{
	ImGui::Begin("Setting",nullptr,ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Light"))
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
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::ColorEdit4("Ambient##Material", &mMaterial.ambient.x);
		ImGui::ColorEdit4("Diffuse##Material", &mMaterial.diffuse.x);
		ImGui::ColorEdit4("Specular##Material", &mMaterial.specular.x);
		ImGui::DragFloat("Power##Material", &mMaterial.power, 1.0f, 1.0f, 100.0f);
	}
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::DragFloat3("Rotation##Transform", &mRotation.x, 0.01f);
	}
	ImGui::End();
}