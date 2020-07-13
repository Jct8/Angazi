#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

namespace
{
	float numRows = 5;
	float numColumns = 5;
	bool normal = true;
	bool aoMap = true;
	float displacement = 0.0f;
	float brightness = 1.0f;

	std::vector<std::string> textureNames;
	std::string choosenTexture;
}

void GameState::SetPBRTextures(std::string textureName)
{
	if (mTextures[textureName].count("Albedo") == 1)
		mPbrEffect.SetDiffuseTexture(mTextures[textureName]["Albedo"].get());
	if (mTextures[textureName].count("Normal") == 1)
	{
		mPbrEffect.SetNormalTexture(mTextures[textureName]["Normal"].get());
		mPbrEffect.SetNormalMapWeight(normal== true ? 1.0f : 0.0f);
	}
	if (mTextures[textureName].count("AO") == 1)
	{
		mPbrEffect.SetAOTexture(mTextures[textureName]["AO"].get());
		mPbrEffect.SetAOWeight(aoMap == true ? 1.0f : 0.0f);
	}
	if (mTextures[textureName].count("Displacement") == 1)
	{
		mPbrEffect.SetDisplacementTexture(mTextures[textureName]["Displacement"].get());
		mPbrEffect.SetBumpMapWeight(displacement);
	}
	if (mTextures[textureName].count("Roughness") == 1)
		mPbrEffect.SetRoughnessTexture(mTextures[textureName]["Roughness"].get());
	if (mTextures[textureName].count("Metallic") == 1)
		mPbrEffect.SetMetallicTexture(mTextures[textureName]["Metallic"].get());
}
void GameState::SetStandardTextures(std::string textureName)
{
	if (mTextures[textureName].count("Albedo") == 1)
		mStandardEffect.SetDiffuseTexture(mTextures[textureName]["Albedo"].get());
	if (mTextures[textureName].count("Normal") == 1)
	{
		mStandardEffect.SetNormalTexture(mTextures[textureName]["Normal"].get());
		mStandardEffect.SetNormalMapWeight(normal == true ? 1.0f : 0.0f);
	}
	if (mTextures[textureName].count("AO") == 1)
	{
		mStandardEffect.SetAOTexture(mTextures[textureName]["AO"].get());
		mStandardEffect.SetAOWeight(aoMap == true ? 1.0f : 0.0f);
	}
	if (mTextures[textureName].count("Displacement") == 1)
	{
		mStandardEffect.SetDisplacementTexture(mTextures[textureName]["Displacement"].get());
		mStandardEffect.SetBumpMapWeight(displacement);
	}
}
void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetPosition({ 0.0f, 0.5f,-2.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });

	mDirectionalLight.direction = Normalize({ 0.0f, 0.0f,1.0f });
	//mDirectionalLight.ambient = { 0.947f,0.888f,0.888f,0.3f };
	mDirectionalLight.ambient = {0.0f};//{ 0.168f ,0.107f,0.107f ,0.3f };
	mDirectionalLight.diffuse = { 0.7f };
	mDirectionalLight.specular = { 0.5f };

	mMaterial.ambient = { 0.496f ,0.379f,0.379f ,0.3f };
	mMaterial.diffuse = { 0.7f };
	mMaterial.specular = { 0.5f };
	mMaterial.power = 80.0f;

	ObjLoader::Load("../../Assets/Models/Mandalorian_Helmet/Mandalorian_Helmet.obj",0.01f, mMesh);
	mMeshBufferHelmet.Initialize(mMesh);
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Mirror);

	mPbrEffect.Initialize();
	mStandardEffect.Initialize();

	//mSkybox.ChangeDefualtSkybox(2);
	//mSkybox.CreateSkybox();
	//mSkybox.CreateSkybox("../../Assets/Images/HdrMaps/Helipad_GoldenHour/LA_Downtown_Helipad_GoldenHour_3k.hdr");
	mSkybox.CreateSkybox("../../Assets/Images/HdrMaps/Shiodome_Stairs/10-Shiodome_Stairs_3k.hdr");

	mPlainTexture.Initialize("../../Assets/Images/white.jpg");

	mTextures["Helmet"]["Albedo"] = std::make_unique<Texture>();
	mTextures["Helmet"]["Albedo"]->Initialize("../../Assets/Models/Mandalorian_Helmet/mandalorianUV2_Helmet_BaseColor.png", true);
	mTextures["Helmet"]["Normal"] = std::make_unique<Texture>();
	mTextures["Helmet"]["Normal"]->Initialize("../../Assets/Models/Mandalorian_Helmet/mandalorianUV2_Helmet_Normal.png");
	mTextures["Helmet"]["AO"] = std::make_unique<Texture>();
	mTextures["Helmet"]["AO"]->Initialize("../../Assets/Models/Mandalorian_Helmet/mandalorianUV2_Helmet_AO.png");
	mTextures["Helmet"]["Displacement"] = std::make_unique<Texture>();
	mTextures["Helmet"]["Displacement"]->Initialize("../../Assets/Models//Mandalorian_Helmet/mandalorianUV2_Helmet_Height.png");
	mTextures["Helmet"]["Roughness"] = std::make_unique<Texture>();
	mTextures["Helmet"]["Roughness"]->Initialize("../../Assets/Models/Mandalorian_Helmet/mandalorianUV2_Helmet_Roughness2.png");
	mTextures["Helmet"]["Metallic"] = std::make_unique<Texture>();
	mTextures["Helmet"]["Metallic"]->Initialize("../../Assets/Models/Mandalorian_Helmet/mandalorianUV2_Helmet_Metallic2.png");

	for (auto &textures : mTextures)
		textureNames.push_back(textures.first);
	choosenTexture = textureNames[0];

	mHdrEffect.Initialize();
}

void GameState::Terminate()
{
	mHdrEffect.Terminate();

	for (auto &textures : mTextures)
	{
		for (auto &texture : textures.second)
			texture.second->Terminate();
	}

	mPlainTexture.Terminate();
	mSkybox.Terminate();
	mSampler.Terminate();

	mStandardEffect.Terminate();
	mPbrEffect.Terminate();

	mMeshBufferHelmet.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 3.0f;
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

void GameState::DrawScene()
{
	auto matRot = Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y) * Matrix4::RotationZ(mRotation.z);
	auto matWorld = matRot * Matrix4::Translation({ 0.0f,0.0f,0.0f });
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();
	mSampler.BindPS();
	mSampler.BindVS();
	if (useTextureMap)
	{
		matWorld = matRot * Matrix4::Translation({ -0.3f,0.0f,0.0f });

		mPbrEffect.Begin();
		mPbrEffect.SetDirectionalLight(mDirectionalLight);
		mPbrEffect.SetMaterial(mMaterial);
		SetPBRTextures(choosenTexture);
		mPbrEffect.SetIrradianceMap(mSkybox.GetIrradianceMap());
		mPbrEffect.SetTransformData(matWorld, matView, matProj, mCamera.GetPosition());
		mPbrEffect.UpdateSettings();
		mMeshBufferHelmet.Draw();
		mPbrEffect.End();

		matWorld = matRot * Matrix4::Translation({ 0.3f,0.0f,0.0f });
		mStandardEffect.Begin();
		mStandardEffect.SetDirectionalLight(mDirectionalLight);
		mStandardEffect.SetMaterial(mMaterial);
		SetStandardTextures(choosenTexture);
		mStandardEffect.SetTransformData(matWorld, matView, matProj, mCamera.GetPosition());
		mStandardEffect.UpdateSettings();
		mMeshBufferHelmet.Draw();
		mStandardEffect.End();
	}
	else
	{
		mPbrEffect.Begin();
		mPbrEffect.SetDirectionalLight(mDirectionalLight);
		mPbrEffect.SetMaterial(mMaterial);
		mPbrEffect.SetIrradianceMap(mSkybox.GetIrradianceMap());
		mPbrEffect.SetDiffuseTexture(&mPlainTexture);
		float spacing = 0.5f;
		for (int row = 0; row < numRows; ++row)
		{
			mPbrEffect.SetMetallicWeight((float)row / (float)numRows);
			for (int col = 0; col < numColumns; ++col)
			{
				mPbrEffect.SetRoughnessWeight(Math::Clamp((float)col / (float)numColumns, 0.05f, 1.0f));
				matWorld = Matrix4::Scaling(0.5f) * Matrix4::Translation({ (col - (numColumns / 2)) * spacing, (row - (numRows / 2)) * spacing, 0.0f });
				mPbrEffect.SetTransformData(matWorld, matView, matProj, mCamera.GetPosition());
				mPbrEffect.UpdateSettings();
				mMeshBufferHelmet.Draw();
			}
		}
		mPbrEffect.End();
	}
	mSkybox.Draw(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %.2f", Angazi::Core::TimeUtil::GetFramesPerSecond());
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
		ImGui::DragFloat("Power##Material", &mMaterial.power, 0.2f, 0.0f, 100.0f);
	}
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("Rotation##Transform", &mRotation.x, 0.01f);
	}
	if (ImGui::CollapsingHeader("PBR Settings Map", ImGuiTreeNodeFlags_DefaultOpen))
	{

		if (ImGui::Checkbox("Show Texture Map", &useTextureMap))
		{
			if (useTextureMap)
			{
				mPbrEffect.SetRoughnessWeight(-1.0f);
				mPbrEffect.SetMetallicWeight(-1.0f);
				mPbrEffect.SetNormalMapWeight(1.0f);
				mPbrEffect.SetAOWeight(1.0f);
				mPbrEffect.SetBumpMapWeight(displacement);
			}
			else
			{
				mPbrEffect.SetNormalMapWeight(0.0f);
				mPbrEffect.SetAOWeight(0.0f);
				mPbrEffect.SetBumpMapWeight(displacement);
			}
		}
		if (ImGui::Checkbox("Normal Map", &normal))
			mPbrEffect.SetNormalMapWeight(normal ? 1.0f : 0.0f);
		if (ImGui::Checkbox("Ambient occlusion", &aoMap))
			mPbrEffect.SetAOWeight(aoMap ? 1.0f : 0.0f);
		if (ImGui::DragFloat("Height Map", &displacement,0.001f, 0.0f, 1.0f))
			mPbrEffect.SetBumpMapWeight(displacement);
		if (ImGui::DragFloat("Brightness", &brightness, 0.01f))
		{
			mPbrEffect.SetBrightness(brightness);
		}

		static const char* item_current = textureNames[0].c_str();
		if (ImGui::BeginCombo("Select Texture",item_current))
		{
			for (size_t n = 0; n < textureNames.size(); n++)
			{
				bool is_selected = (item_current == textureNames[n].c_str());
				if (ImGui::Selectable(textureNames[n].c_str(), is_selected))
				{
					item_current = textureNames[n].c_str();
					choosenTexture = textureNames[n];
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

	}
	ImGui::End();
}