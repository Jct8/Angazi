#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;

namespace
{
	float displacementWeight = 0.0f;
	float brightness = 1.0f;
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);


	mDirectionalLight.direction = Normalize({ 0.327f,-0.382f, 0.864f });
	mDirectionalLight.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mDirectionalLight.diffuse = { 0.75f,0.75f,0.75f ,1.0f };
	mDirectionalLight.specular = { 0.5f,0.5f,0.5f ,1.0f };

	mMaterial.ambient = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.diffuse = { 0.8f,0.8f,0.8f ,1.0f };
	mMaterial.specular = { 0.5f,0.5f,0.5f ,1.0f };
	mMaterial.power = 80.0f;

	mMeshSphere = MeshBuilder::CreateSpherePX(2, 32, 32);
	mMeshBufferSphere.Initialize(mMeshSphere);

	mConstantBuffer.Initialize(sizeof(Matrix4));

	mVertexShader.Initialize("../../Assets/Shaders/DoTexturing.fx", VertexPX::Format);
	mPixelShader.Initialize("../../Assets/Shaders/DoTexturing.fx");
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Wrap);

	for (int i = 0; i < 10; i++)
	{
		mPlanetTextures.emplace_back();
		mSelfRotationSpeed.emplace_back();
		mRotationSpeed.emplace_back();
		mScale.emplace_back();
		mTranslation.emplace_back();
		mMoonTextures.emplace_back();
		mMoonTextures[i].Initialize("../../Assets/Images/Space/Moon.jpg");
	}

	mPlanetTextures[0].Initialize("../../Assets/Images/Space/Sun.jpg");
	mPlanetTextures[1].Initialize("../../Assets/Images/Space/Mercury.jpg");
	mPlanetTextures[2].Initialize("../../Assets/Images/Space/Venus.jpg");
	mPlanetTextures[3].Initialize("../../Assets/Images/Space/Earth.jpg");
	mPlanetTextures[4].Initialize("../../Assets/Images/Space/Mars.jpg");
	mPlanetTextures[5].Initialize("../../Assets/Images/Space/Jupiter.jpg");
	mPlanetTextures[6].Initialize("../../Assets/Images/Space/Saturn.jpg");
	mPlanetTextures[7].Initialize("../../Assets/Images/Space/Uranus.jpg");
	mPlanetTextures[8].Initialize("../../Assets/Images/Space/Neptune.jpg");
	mPlanetTextures[9].Initialize("../../Assets/Images/Space/Pluto.jpg");

	mSelfRotationSpeed[0] = 0.0f;
	mSelfRotationSpeed[1] = 0.2f;
	mSelfRotationSpeed[2] = 1.4f;
	mSelfRotationSpeed[3] = 1.0f;
	mSelfRotationSpeed[4] = 1.4f;
	mSelfRotationSpeed[5] = 0.8f;
	mSelfRotationSpeed[6] = 0.4f;
	mSelfRotationSpeed[7] = 0.6f;
	mSelfRotationSpeed[8] = 1.2f;
	mSelfRotationSpeed[9] = 0.4f;

	mRotationSpeed[0] = 0.0f;
	mRotationSpeed[1] = 1.36f;
	mRotationSpeed[2] = 1.16f;
	mRotationSpeed[3] = 0.96f;
	mRotationSpeed[4] = 0.80f;
	mRotationSpeed[5] = 0.63f;
	mRotationSpeed[6] = 0.50f;
	mRotationSpeed[7] = 0.40f;
	mRotationSpeed[8] = 0.3f;
	mRotationSpeed[9] = 0.56f;

	mScale[0] = 2.0f;
	mScale[1] = 0.4f;
	mScale[2] = 0.9f;
	mScale[3] = 0.9f;
	mScale[4] = 0.6f;
	mScale[5] = 1.4f;
	mScale[6] = 1.2f;
	mScale[7] = 1.0f;
	mScale[8] = 1.0f;
	mScale[9] = 0.3f;

	mSkybox.AddTexture("../../Assets/Images/SpaceSkybox/back.png", Skybox::Back);
	mSkybox.AddTexture("../../Assets/Images/SpaceSkybox/front.png", Skybox::Front);
	mSkybox.AddTexture("../../Assets/Images/SpaceSkybox/left.png", Skybox::Left);
	mSkybox.AddTexture("../../Assets/Images/SpaceSkybox/right.png", Skybox::Right);
	mSkybox.AddTexture("../../Assets/Images/SpaceSkybox/top.png", Skybox::Top);
	mSkybox.AddTexture("../../Assets/Images/SpaceSkybox/bottom.png", Skybox::Bottom);
	mSkybox.CreateSkybox();

	ObjLoader::Load("../../Assets/Models/Spaceship/Trident-A10.obj", 0.001f, mSpaceshipMesh);
	mMeshBufferSpaceship.Initialize(mSpaceshipMesh);
	mStandardEffect.Initialize();
	mStandardEffect.SetDiffuseTexture("../../Assets/Models/Spaceship/Trident_Dekol_Color.png");
	mStandardEffect.SetDisplacementTexture("../../Assets/Models/Spaceship/Trident_Bump.png");
	mStandardEffect.SetSpecularTexture("../../Assets/Models/Spaceship/Trident_Specular.png");

	mStandardEffect.SetBumpMapWeight(displacementWeight);
	mStandardEffect.SetBrightness(brightness);

	mShipPosition = 0.0f;
	mShipDirection = Vector3::ZAxis;

	// Snap camea to target position by default
	mCameraTargetPosition = mShipPosition - (mShipDirection * mCameraDistanceOffset) + (Vector3::YAxis * mCameraHeightOffset);
	mCamera.SetPosition(mCameraTargetPosition);
	mCamera.SetDirection(mShipDirection);

	for (size_t i = 0; i < mThrusters.size(); i++)
	{
		mThrusters[i].Initialize("../../Assets/Images/Particles/Particle1.jpg");
		mThrusters[i].SetStartColor(Colors::Red);
		mThrusters[i].SetEndColor(Colors::Yellow);
		mThrusters[i].SetStartSize(0.2f);
		mThrusters[i].SetEndSize(0.0f);
	}

}

void GameState::Terminate()
{
	mThrusters[0].Terminate();
	mThrusters[1].Terminate();
	mStandardEffect.Terminate();
	mMeshBufferSpaceship.Terminate();
	mSkybox.Terminate();
	for (size_t i = 0; i < mPlanetTextures.size(); i++)
	{
		mPlanetTextures[i].Terminate();
		mMoonTextures[i].Terminate();
	}

	mSampler.Terminate();
	mMeshBufferSphere.Terminate();
	mConstantBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void GameState::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();
	const float kMoveSpeed = inputSystem->IsKeyDown(KeyCode::LSHIFT) ? 100.0f : 10.0f;
	const float kTurnSpeed = 10.0f * Constants::DegToRad;

	mShipTilt *= 0.95f;

	const float shipTurnSpeed = 0.3f;
	mShipRotation += inputSystem->GetMouseMoveX() * shipTurnSpeed * deltaTime;
	mShipElevation += inputSystem->GetMouseMoveY() * shipTurnSpeed * deltaTime;

	auto yawRotation = Math::Matrix4::RotationY(mShipRotation);
	mShipDirection = Math::TransformNormal(Vector3::ZAxis, yawRotation);

	const Math::Vector3 right = Math::Normalize(Cross(Math::Vector3::YAxis, mShipDirection));
	const Math::Matrix4 pitchRotation = Math::Matrix4::RotationAxis(right, mShipElevation);
	mShipDirection = Math::Normalize(Math::TransformNormal(mShipDirection, pitchRotation));

	const float shipMoveSpeed = 5.0f;
	if (inputSystem->IsKeyDown(KeyCode::W))
		mShipPosition += mShipDirection * shipMoveSpeed* deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::S))
		mShipPosition -= mShipDirection * shipMoveSpeed* deltaTime;
	if (inputSystem->IsKeyDown(KeyCode::D))
	{
		mShipTilt = Clamp(mShipTilt - (1.0f * deltaTime), -0.5f, 0.5f);
		mShipPosition += right * shipMoveSpeed * deltaTime;
	}
	if (inputSystem->IsKeyDown(KeyCode::A))
	{
		mShipTilt = Clamp(mShipTilt + (1.0f * deltaTime), -0.5f, 0.5f);
		mShipPosition -= right * shipMoveSpeed* deltaTime;
	}
	mCameraTargetPosition = mShipPosition - (mShipDirection * mCameraDistanceOffset) + (Vector3::YAxis * mCameraHeightOffset);
	auto cameraPosition = Lerp(mCamera.GetPosition(), mCameraTargetPosition, 0.2f);
	mCamera.SetPosition(cameraPosition);
	mCamera.SetDirection(mShipDirection);

	mRotation -= deltaTime;
	mThrusters[0].SetPosition(mShipPosition - (mShipDirection *0.5f) - (right*0.18f));
	mThrusters[1].SetPosition(mShipPosition - (mShipDirection *0.5f) + (right*0.18f));

	for (auto& thruster : mThrusters)
	{
		thruster.Start(100.0f);
		thruster.Update(deltaTime);
	}
}

void GameState::Render()
{
	mSkybox.Draw(mCamera);

	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();

	const Math::Vector3 l = mShipDirection;
	const Math::Vector3 r = Math::Normalize(Math::Cross(Math::Vector3::YAxis, l));
	const Math::Vector3 u = Math::Normalize(Math::Cross(l, r));
	const Math::Vector3 p = mShipPosition;

	auto matTilt = Matrix4::RotationZ(mShipTilt);
	auto matWorld = Matrix4
	{
		r.x, r.y, r.z, 0.0f,
		u.x, u.y, u.z, 0.0f,
		l.x, l.y, l.z, 0.0f,
		p.x, p.y, p.z, 1.0f
	};

	mStandardEffect.Begin();
	mStandardEffect.SetMaterial(mMaterial);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetTransformData( matTilt * matWorld, matView, matProj, mCamera.GetPosition());
	mStandardEffect.UpdateSettings();
	mMeshBufferSpaceship.Draw();
	mStandardEffect.End();

	for(auto& thruster : mThrusters)
		thruster.Draw(mCamera);

	mConstantBuffer.BindVS();
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindVS();

	for (size_t i = 0; i < mPlanetTextures.size(); i++)
	{
		//Planets
		auto matWorld = Matrix4::RotationY(mRotation*mRotationSpeed[i]);
		auto matSelfRotation = Matrix4::RotationY(mRotation*mSelfRotationSpeed[i]);
		auto matScale = Matrix4::Scaling(mScale[i]);
		auto matTrans = Matrix4::Translation({ 0.5f*i + i * 5.0f,0.0f,0.0f });
		auto matWVP = Transpose(matScale * matSelfRotation * matTrans * matWorld * matView * matProj);

		mConstantBuffer.Update(&matWVP);
		mPlanetTextures[i].BindVS();
		mPlanetTextures[i].BindPS();
		mMeshBufferSphere.Draw();

		//Moon
		auto matMoon = Matrix4::RotationY(mRotation*0.4f);
		auto matMoonSelfRotation = Matrix4::RotationY(mRotation*10.5f);
		auto matMoonScale = Matrix4::Scaling(0.1f);
		auto matMoonTrans = Matrix4::Translation({ 2.0f,0.0f,0.0f });
		auto matMoonWVP = Transpose(matMoonScale *matMoonSelfRotation * matMoonTrans * matMoon * matTrans * matWorld * matView * matProj);

		mMoonTextures[i].BindVS();
		mMoonTextures[i].BindPS();
		mConstantBuffer.Update(&matMoonWVP);
		mMeshBufferSphere.Draw();
	}

	SimpleDraw::AddGroundPlane(50);
	SimpleDraw::Render(mCamera);
}

void GameState::DebugUI()
{
	ImGui::Begin("Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %.2f", Angazi::Core::TimeUtil::GetFramesPerSecond());
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
		static bool specular = true;

		if (ImGui::Checkbox("Specular Map", &specular))
		{
			float specularMapWeight = specular ? 1.0f : 0.0f;
			mStandardEffect.SetSpecularMapWeight(specularMapWeight);
		}
		if (ImGui::SliderFloat("Displacement", &displacementWeight, 0.0f, 10.0f))
			mStandardEffect.SetBumpMapWeight(displacementWeight);
		if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 10.f))
			mStandardEffect.SetBrightness(brightness);
	}
	ImGui::End();
}