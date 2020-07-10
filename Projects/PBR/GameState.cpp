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
	float displacement = 0.1f;
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

	mCamera.SetPosition({ 0.0f, 0.0f,-4.0f });
	mCamera.SetDirection({ 0.0f,0.0f,1.0f });

	mDirectionalLight.direction = Normalize({ 0.0f, 0.0f,1.0f });
	//mDirectionalLight.ambient = { 0.947f,0.888f,0.888f,0.3f };
	mDirectionalLight.ambient = { 0.168f ,0.107f,0.107f ,0.3f };
	mDirectionalLight.diffuse = { 0.7f };
	mDirectionalLight.specular = { 0.5f };

	mMaterial.ambient = { 0.496f ,0.379f,0.379f ,0.3f };
	mMaterial.diffuse = { 0.7f };
	mMaterial.specular = { 0.5f };
	mMaterial.power = 80.0f;

	mMeshBufferSphere.Initialize(MeshBuilder::CreateSphere(1.0f, 256, 256,2));
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Mirror);

	mPbrEffect.Initialize();
	mStandardEffect.Initialize();

	mSkybox.ChangeDefualtSkybox(2);
	mSkybox.CreateSkybox();

	mPlainTexture.Initialize("../../Assets/Images/white.jpg");

	mTextures["Rock"]["Albedo"] = std::make_unique<Texture>();
	mTextures["Rock"]["Albedo"]->Initialize("../../Assets/Images/PBR/4K_Rock/Rock034_4K_Color.jpg",true);
	mTextures["Rock"]["Normal"] = std::make_unique<Texture>();
	mTextures["Rock"]["Normal"]->Initialize("../../Assets/Images/PBR/4K_Rock/Rock034_4K_Normal.jpg");
	mTextures["Rock"]["AO"] = std::make_unique<Texture>();
	mTextures["Rock"]["AO"]->Initialize("../../Assets/Images/PBR/4K_Rock/Rock034_4K_AmbientOcclusion.jpg");
	mTextures["Rock"]["Displacement"] = std::make_unique<Texture>();
	mTextures["Rock"]["Displacement"]->Initialize("../../Assets/Images/PBR/4K_Rock/Rock034_4K_Displacement.jpg");
	mTextures["Rock"]["Roughness"] = std::make_unique<Texture>();
	mTextures["Rock"]["Roughness"]->Initialize("../../Assets/Images/PBR/4K_Rock/Rock034_4K_Roughness.jpg");

	mTextures["Planks"]["Albedo"] = std::make_unique<Texture>();
	mTextures["Planks"]["Albedo"]->Initialize("../../Assets/Images/PBR/4K_Planks/Planks019_4K_Color.jpg",true);
	mTextures["Planks"]["Normal"] = std::make_unique<Texture>();
	mTextures["Planks"]["Normal"]->Initialize("../../Assets/Images/PBR/4K_Planks/Planks019_4K_Normal.jpg");
	mTextures["Planks"]["AO"] = std::make_unique<Texture>();
	mTextures["Planks"]["AO"]->Initialize("../../Assets/Images/PBR/4K_Planks/Planks019_4K_AmbientOcclusion.jpg");
	mTextures["Planks"]["Displacement"] = std::make_unique<Texture>();
	mTextures["Planks"]["Displacement"]->Initialize("../../Assets/Images/PBR/4K_Planks/Planks019_4K_Displacement.jpg");
	mTextures["Planks"]["Roughness"] = std::make_unique<Texture>();
	mTextures["Planks"]["Roughness"]->Initialize("../../Assets/Images/PBR/4K_Planks/Planks019_4K_Roughness.jpg");
	mTextures["Planks"]["Metallic"] = std::make_unique<Texture>();
	mTextures["Planks"]["Metallic"]->Initialize("../../Assets/Images/PBR/4K_Planks/Planks019_4K_Metalness.jpg");

	mTextures["Weave"]["Albedo"] = std::make_unique<Texture>();
	mTextures["Weave"]["Albedo"]->Initialize("../../Assets/Images/PBR/dirty-wicker-weave1/dirty-wicker-weave1-albedo.png",true);
	mTextures["Weave"]["Normal"] = std::make_unique<Texture>();
	mTextures["Weave"]["Normal"]->Initialize("../../Assets/Images/PBR/dirty-wicker-weave1/dirty-wicker-weave1-normal-dx.png");
	mTextures["Weave"]["AO"] = std::make_unique<Texture>();
	mTextures["Weave"]["AO"]->Initialize("../../Assets/Images/PBR/dirty-wicker-weave1/dirty-wicker-weave1-ao.png");
	mTextures["Weave"]["Displacement"] = std::make_unique<Texture>();
	mTextures["Weave"]["Displacement"]->Initialize("../../Assets/Images/PBR/dirty-wicker-weave1/dirty-wicker-weave1-height.png");
	mTextures["Weave"]["Roughness"] = std::make_unique<Texture>();
	mTextures["Weave"]["Roughness"]->Initialize("../../Assets/Images/PBR/dirty-wicker-weave1/dirty-wicker-weave1-roughness.png");
	mTextures["Weave"]["Metallic"] = std::make_unique<Texture>();
	mTextures["Weave"]["Metallic"]->Initialize("../../Assets/Images/PBR/dirty-wicker-weave1/dirty-wicker-weave1-metallic.png");

	mTextures["Bark"]["Albedo"] = std::make_unique<Texture>();
	mTextures["Bark"]["Albedo"]->Initialize("../../Assets/Images/PBR/bark1/bark1-albedo.png",true);
	mTextures["Bark"]["Normal"] = std::make_unique<Texture>();
	mTextures["Bark"]["Normal"]->Initialize("../../Assets/Images/PBR/bark1/bark1-normal3.png");
	mTextures["Bark"]["AO"] = std::make_unique<Texture>();
	mTextures["Bark"]["AO"]->Initialize("../../Assets/Images/PBR/bark1/bark1-ao.png");
	mTextures["Bark"]["Displacement"] = std::make_unique<Texture>();
	mTextures["Bark"]["Displacement"]->Initialize("../../Assets/Images/PBR/bark1/bark1-height2.png");
	mTextures["Bark"]["Roughness"] = std::make_unique<Texture>();
	mTextures["Bark"]["Roughness"]->Initialize("../../Assets/Images/PBR/bark1/bark1-rough.png");
	mTextures["Bark"]["Metallic"] = std::make_unique<Texture>();
	mTextures["Bark"]["Metallic"]->Initialize("../../Assets/Images/PBR/bark1/bark1-metalness.png");

	mTextures["Concrete"]["Albedo"] = std::make_unique<Texture>();
	mTextures["Concrete"]["Albedo"]->Initialize("../../Assets/Images/PBR/broken_down_concrete2/broken_down_concrete2_albedo.png", true);
	mTextures["Concrete"]["Normal"] = std::make_unique<Texture>();
	mTextures["Concrete"]["Normal"]->Initialize("../../Assets/Images/PBR/broken_down_concrete2/broken_down_concrete2_Normal-dx.png");
	mTextures["Concrete"]["AO"] = std::make_unique<Texture>();
	mTextures["Concrete"]["AO"]->Initialize("../../Assets/Images/PBR/broken_down_concrete2/broken_down_concrete2_ao.png");
	mTextures["Concrete"]["Displacement"] = std::make_unique<Texture>();
	mTextures["Concrete"]["Displacement"]->Initialize("../../Assets/Images/PBR/broken_down_concrete2/broken_down_concrete2_Height.png");
	mTextures["Concrete"]["Roughness"] = std::make_unique<Texture>();
	mTextures["Concrete"]["Roughness"]->Initialize("../../Assets/Images/PBR/broken_down_concrete2/broken_down_concrete2_Roughness.png");
	mTextures["Concrete"]["Metallic"] = std::make_unique<Texture>();
	mTextures["Concrete"]["Metallic"]->Initialize("../../Assets/Images/PBR/broken_down_concrete2/broken_down_concrete2_Metallic.png");

	mTextures["Cobblestylized"]["Albedo"] = std::make_unique<Texture>();
	mTextures["Cobblestylized"]["Albedo"]->Initialize("../../Assets/Images/PBR/cobblestylized3/cobblestylized3_albedo.png", true);
	mTextures["Cobblestylized"]["Normal"] = std::make_unique<Texture>();
	mTextures["Cobblestylized"]["Normal"]->Initialize("../../Assets/Images/PBR/cobblestylized3/cobblestylized3_normal-dx.png");
	mTextures["Cobblestylized"]["AO"] = std::make_unique<Texture>();
	mTextures["Cobblestylized"]["AO"]->Initialize("../../Assets/Images/PBR/cobblestylized3/cobblestylized3_ao.png");
	mTextures["Cobblestylized"]["Displacement"] = std::make_unique<Texture>();
	mTextures["Cobblestylized"]["Displacement"]->Initialize("../../Assets/Images/PBR/cobblestylized3/cobblestylized3_height.png");
	mTextures["Cobblestylized"]["Roughness"] = std::make_unique<Texture>();
	mTextures["Cobblestylized"]["Roughness"]->Initialize("../../Assets/Images/PBR/cobblestylized3/cobblestylized3_roughness.png");
	mTextures["Cobblestylized"]["Metallic"] = std::make_unique<Texture>();
	mTextures["Cobblestylized"]["Metallic"]->Initialize("../../Assets/Images/PBR/cobblestylized3/cobblestylized3_metallic.png");

	mTextures["Gray bricks"]["Albedo"] = std::make_unique<Texture>();
	mTextures["Gray bricks"]["Albedo"]->Initialize("../../Assets/Images/PBR/gray-bricks1/gray-bricks1-albedo.png", true);
	mTextures["Gray bricks"]["Normal"] = std::make_unique<Texture>();
	mTextures["Gray bricks"]["Normal"]->Initialize("../../Assets/Images/PBR/gray-bricks1/gray-bricks1-Normal-dx.png");
	mTextures["Gray bricks"]["AO"] = std::make_unique<Texture>();
	mTextures["Gray bricks"]["AO"]->Initialize("../../Assets/Images/PBR/gray-bricks1/gray-bricks1-ao.png");
	mTextures["Gray bricks"]["Displacement"] = std::make_unique<Texture>();
	mTextures["Gray bricks"]["Displacement"]->Initialize("../../Assets/Images/PBR/gray-bricks1/gray-bricks1-Height.png");
	mTextures["Gray bricks"]["Roughness"] = std::make_unique<Texture>();
	mTextures["Gray bricks"]["Roughness"]->Initialize("../../Assets/Images/PBR/gray-bricks1/gray-bricks1-Roughness.png");
	mTextures["Gray bricks"]["Metallic"] = std::make_unique<Texture>();
	mTextures["Gray bricks"]["Metallic"]->Initialize("../../Assets/Images/PBR/gray-bricks1/gray-bricks1-Metallic.png");

	mTextures["Modern Tile"]["Albedo"] = std::make_unique<Texture>();
	mTextures["Modern Tile"]["Albedo"]->Initialize("../../Assets/Images/PBR/modern-tile1/modern-tile1-albedo.png", true);
	mTextures["Modern Tile"]["Normal"] = std::make_unique<Texture>();
	mTextures["Modern Tile"]["Normal"]->Initialize("../../Assets/Images/PBR/modern-tile1/modern-tile1-normal-dx.png");
	mTextures["Modern Tile"]["AO"] = std::make_unique<Texture>();
	mTextures["Modern Tile"]["AO"]->Initialize("../../Assets/Images/PBR/modern-tile1/modern-tile1-ao.png");
	mTextures["Modern Tile"]["Displacement"] = std::make_unique<Texture>();
	mTextures["Modern Tile"]["Displacement"]->Initialize("../../Assets/Images/PBR/modern-tile1/modern-tile1-height.png");
	mTextures["Modern Tile"]["Roughness"] = std::make_unique<Texture>();
	mTextures["Modern Tile"]["Roughness"]->Initialize("../../Assets/Images/PBR/modern-tile1/modern-tile1-roughness.png");
	mTextures["Modern Tile"]["Metallic"] = std::make_unique<Texture>();
	mTextures["Modern Tile"]["Metallic"]->Initialize("../../Assets/Images/PBR/modern-tile1/modern-tile1-metallic.png");

	mTextures["Ornate celtic gold"]["Albedo"] = std::make_unique<Texture>();
	mTextures["Ornate celtic gold"]["Albedo"]->Initialize("../../Assets/Images/PBR/ornate-celtic-gold/ornate-celtic-gold-albedo.png", true);
	mTextures["Ornate celtic gold"]["Normal"] = std::make_unique<Texture>();
	mTextures["Ornate celtic gold"]["Normal"]->Initialize("../../Assets/Images/PBR/ornate-celtic-gold/ornate-celtic-gold-normal-dx.png");
	mTextures["Ornate celtic gold"]["AO"] = std::make_unique<Texture>();
	mTextures["Ornate celtic gold"]["AO"]->Initialize("../../Assets/Images/PBR/ornate-celtic-gold/ornate-celtic-gold-ao.png");
	mTextures["Ornate celtic gold"]["Displacement"] = std::make_unique<Texture>();
	mTextures["Ornate celtic gold"]["Displacement"]->Initialize("../../Assets/Images/PBR/ornate-celtic-gold/ornate-celtic-gold-height.png");
	mTextures["Ornate celtic gold"]["Roughness"] = std::make_unique<Texture>();
	mTextures["Ornate celtic gold"]["Roughness"]->Initialize("../../Assets/Images/PBR/ornate-celtic-gold/ornate-celtic-gold-roughness.png");
	mTextures["Ornate celtic gold"]["Metallic"] = std::make_unique<Texture>();
	mTextures["Ornate celtic gold"]["Metallic"]->Initialize("../../Assets/Images/PBR/ornate-celtic-gold/ornate-celtic-gold-metallic.png");

	mTextures["Stylized Cliff"]["Albedo"] = std::make_unique<Texture>();
	mTextures["Stylized Cliff"]["Albedo"]->Initialize("../../Assets/Images/PBR/stylized-cliff2/stylized-cliff2_albedo.png", true);
	mTextures["Stylized Cliff"]["Normal"] = std::make_unique<Texture>();
	mTextures["Stylized Cliff"]["Normal"]->Initialize("../../Assets/Images/PBR/stylized-cliff2/stylized-cliff2_normal-dx.png");
	mTextures["Stylized Cliff"]["AO"] = std::make_unique<Texture>();
	mTextures["Stylized Cliff"]["AO"]->Initialize("../../Assets/Images/PBR/stylized-cliff2/stylized-cliff2_ao.png");
	mTextures["Stylized Cliff"]["Displacement"] = std::make_unique<Texture>();
	mTextures["Stylized Cliff"]["Displacement"]->Initialize("../../Assets/Images/PBR/stylized-cliff2/stylized-cliff2_height.png");
	mTextures["Stylized Cliff"]["Roughness"] = std::make_unique<Texture>();
	mTextures["Stylized Cliff"]["Roughness"]->Initialize("../../Assets/Images/PBR/stylized-cliff2/stylized-cliff2_roughness.png");
	mTextures["Stylized Cliff"]["Metallic"] = std::make_unique<Texture>();
	mTextures["Stylized Cliff"]["Metallic"]->Initialize("../../Assets/Images/PBR/stylized-cliff2/stylized-cliff2_metallic.png");

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

	mMeshBufferSphere.Terminate();
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
		matWorld = matRot * Matrix4::Translation({ -1.5f,0.0f,0.0f });

		mPbrEffect.Begin();
		mPbrEffect.SetDirectionalLight(mDirectionalLight);
		mPbrEffect.SetMaterial(mMaterial);
		SetPBRTextures(choosenTexture);
		mPbrEffect.SetTransformData(matWorld, matView, matProj, mCamera.GetPosition());
		mPbrEffect.UpdateSettings();
		mMeshBufferSphere.Draw();
		mPbrEffect.End();

		matWorld = matRot * Matrix4::Translation({ 1.5f,0.0f,0.0f });
		mStandardEffect.Begin();
		mStandardEffect.SetDirectionalLight(mDirectionalLight);
		mStandardEffect.SetMaterial(mMaterial);
		SetStandardTextures(choosenTexture);
		mStandardEffect.SetTransformData(matWorld, matView, matProj, mCamera.GetPosition());
		mStandardEffect.UpdateSettings();
		mMeshBufferSphere.Draw();
		mStandardEffect.End();
	}
	else
	{
		mPbrEffect.Begin();
		mPbrEffect.SetDirectionalLight(mDirectionalLight);
		mPbrEffect.SetMaterial(mMaterial);

		mPbrEffect.SetDiffuseTexture(&mPlainTexture);
		float spacing = 1.3f;
		for (int row = 0; row < numRows; ++row)
		{
			mPbrEffect.SetMetallicWeight((float)row / (float)numRows);
			for (int col = 0; col < numColumns; ++col)
			{
				mPbrEffect.SetRoughnessWeight(Math::Clamp((float)col / (float)numColumns, 0.05f, 1.0f));
				matWorld = Matrix4::Scaling(0.5f) * Matrix4::Translation({ (col - (numColumns / 2)) * spacing, (row - (numRows / 2)) * spacing, 0.0f });
				mPbrEffect.SetTransformData(matWorld, matView, matProj, mCamera.GetPosition());
				mPbrEffect.UpdateSettings();
				mMeshBufferSphere.Draw();
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
		if (ImGui::DragFloat("Height Map", &displacement,0.005f, 0.0f, 1.0f))
			mPbrEffect.SetBumpMapWeight(displacement);
		if (ImGui::DragFloat("Brightness", &brightness, 0.01f))
		{
			mPbrEffect.SetBrightness(brightness);
		}

		static const char* item_current = textureNames[0].c_str();
		if (ImGui::BeginCombo("Select Texture",item_current))
		{
			for (int n = 0; n < textureNames.size(); n++)
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