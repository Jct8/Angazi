#include "GameState.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Physics;
using namespace Angazi::Input;
using namespace Angazi::Math;

namespace
{
	int GetIndex(int x, int y, int columns)
	{
		return (y*columns) + x;
	}
}

void GameState::Initialize()
{
	GraphicsSystem::Get()->SetClearColor(Colors::Black);

	mCamera.SetNearPlane(0.1f);
	mCamera.SetFarPlane(300.0f);
	mCamera.SetPosition({ 0.0f, 23.0f, -33.0f });
	mCamera.SetDirection({ 0.0f,-0.36f, 0.92f });

	Physics::PhysicsWorld::Settings settings;
	settings.drag = 0.1f;
	mPhysicsWorld.Initialize(settings);
	mPhysicsWorld.AddStaticPlane({ Vector3::YAxis,0.0f });
	mPhysicsWorld.AddStaticOBB({ { 2.0f,2.0f,0.0f }, {4.0f,0.2f,5.0f} , Quaternion::RotationAxis(Vector3::ZAxis, 10.0f * Constants::DegToRad) });
	mPhysicsWorld.AddStaticOBB({ {-3.0f,4.5f,0.0f }, {4.0f,0.2f,5.0f} , Quaternion::RotationAxis(Vector3::ZAxis,-15.0f * Constants::DegToRad) });

	mMesh = MeshBuilder::CreatePlanePX(height, width);
	mMeshBuffer.Initialize(mMesh,true);
	mVertexShader.Initialize("../../Assets/Shaders/DoTexturing.fx", VertexPX::Format);
	mPixelShader.Initialize("../../Assets/Shaders/DoTexturing.fx");
	mSampler.Initialize(Sampler::Filter::Anisotropic, Sampler::AddressMode::Border);
	mTexture.Initialize("../../Assets/Images/GOAT.jpg");
	mConstantBuffer.Initialize(sizeof(Matrix4));
}

void GameState::Terminate()
{
	mConstantBuffer.Terminate();
	mTexture.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mMeshBuffer.Terminate();

	mPhysicsWorld.Clear();
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

	mPhysicsWorld.Update(deltaTime);
	if (usingClothTexture && mMesh.vertices.size() == mParticles.size() )
	{
		for (size_t i = 0; i < mParticles.size(); i++)
		{
			mMesh.vertices[i].position = mParticles[i]->position;
		}
	}

	SimpleDraw::AddGroundPlane(50);
}

void GameState::Render()
{
	DrawScene();
}

void GameState::DebugUI()
{
	ImGui::Begin("Physics", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %.2f", Angazi::Core::TimeUtil::GetFramesPerSecond());
	if (ImGui::Button("Particles"))
	{
		mPhysicsWorld.Clear(true);
		usingClothTexture = false;
		for (int i = 0; i < 100; i++)
		{
			auto p = new Physics::Particle;
			p->SetPosition({ RandomFloat(-3.0f,3.0f) , 10.0f , RandomFloat(-3.0f,3.0f) });
			p->SetVelocity({ RandomFloat(-0.05f,0.05f) ,RandomFloat(-0.1f,0.5f),RandomFloat(-0.05f,0.05f) });
			p->radius = 0.1f;
			p->invMass = 0.01f;
			p->bounce = 0.4f;
			mPhysicsWorld.AddParticles(p);
		}
	}
	if (ImGui::Button("Sticks"))
	{
		mPhysicsWorld.Clear(true);
		usingClothTexture = false;
		for (int i = 0; i < 50; i++)
		{
			auto p1 = new Physics::Particle();
			auto p2 = new Physics::Particle();
			p1->SetPosition({ RandomFloat(-3.0f,3.0f) , 10.0f , RandomFloat(-3.0f,3.0f) });
			p1->SetVelocity({ RandomFloat(-0.05f,0.05f) ,RandomFloat(-0.1f,0.5f),RandomFloat(-0.05f,0.05f) });
			p1->radius = 0.1f;
			p1->bounce = 0.3f;

			p2->SetPosition({ RandomFloat(-3.0f,3.0f) , 10.0f , RandomFloat(-3.0f,3.0f) });
			p2->SetVelocity({ RandomFloat(-0.05f,0.05f) ,RandomFloat(-0.1f,0.5f),RandomFloat(-0.05f,0.05f) });
			p2->radius = 0.1f;
			p2->bounce = 0.3f;
			mPhysicsWorld.AddParticles(p1);
			mPhysicsWorld.AddParticles(p2);
			auto c = new Physics::Spring(p1, p2);
			mPhysicsWorld.AddConstraint(c);
		}
	}
	if (ImGui::Button("Tetrahedron"))
	{
		mPhysicsWorld.Clear(true);
		usingClothTexture = false;
		for (int i = 0; i < 50; i++)
		{
			auto p1 = new Physics::Particle();
			auto p2 = new Physics::Particle();
			auto p3 = new Physics::Particle();
			auto p4 = new Physics::Particle();

			p1->SetPosition({ -3.0f , 10.0f , -3.0f });
			p1->SetVelocity({ RandomFloat(-0.05f,0.01f) ,RandomFloat(-0.1f,0.5f),RandomFloat(-0.05f,0.05f) });
			p1->radius = 0.1f;
			p1->bounce = 0.3f;

			p2->SetPosition({ 3.0f , 10.0f , -3.0f });
			p2->SetVelocity({ RandomFloat(-0.05f,0.01f) ,RandomFloat(-0.1f,0.5f),RandomFloat(-0.05f,0.05f) });
			p2->radius = 0.1f;
			p2->bounce = 0.3f;

			p3->SetPosition({ 0.0f , 10.0f , 3.0f });
			p3->SetVelocity({ RandomFloat(-0.05f,0.01f) ,RandomFloat(-0.1f,0.5f),RandomFloat(-0.05f,0.05f) });
			p3->radius = 0.1f;
			p3->bounce = 0.3f;

			p4->SetPosition({ 0.0f , 10.0f , 0.0f });
			p4->SetVelocity({ RandomFloat(-0.05f,0.01f) ,RandomFloat(-0.1f,0.5f),RandomFloat(-0.05f,0.05f) });
			p4->radius = 0.1f;
			p4->bounce = 0.3f;

			mPhysicsWorld.AddParticles(p1);
			mPhysicsWorld.AddParticles(p2);
			mPhysicsWorld.AddParticles(p3);
			mPhysicsWorld.AddParticles(p4);

			auto c = new Physics::Spring(p1, p2, 1.0f);
			auto c2 = new Physics::Spring(p2, p3, 1.0f);
			auto c3 = new Physics::Spring(p3, p1, 1.0f);
			auto c4 = new Physics::Spring(p1, p4, 1.0f);
			auto c5 = new Physics::Spring(p2, p4, 1.0f);
			auto c6 = new Physics::Spring(p3, p4, 1.0f);
			mPhysicsWorld.AddConstraint(c);
			mPhysicsWorld.AddConstraint(c2);
			mPhysicsWorld.AddConstraint(c3);
			mPhysicsWorld.AddConstraint(c4);
			mPhysicsWorld.AddConstraint(c5);
			mPhysicsWorld.AddConstraint(c6);
		}
	}
	if (ImGui::Button("Cube"))
	{
		mParticles.clear();
		usingClothTexture = false;
		mPhysicsWorld.Clear(true);
		Vector3 center = Vector3::Zero;
		center.y = 10.0f;
		Vector3 extend = Vector3::One;
		for (int i = 0; i < 20; i++)
		{
			for (int i = 0; i < 8; i++)
			{
				mParticles.push_back(new Physics::Particle());
				mParticles.back()->radius = 0.1f;
				mParticles.back()->bounce = 0.3f;
				mPhysicsWorld.AddParticles(mParticles.back());
			}
			const float minX = center.x - extend.x;
			const float minY = center.y - extend.y;
			const float minZ = center.z - extend.z;
			const float maxX = center.x + extend.x;
			const float maxY = center.y + extend.y;
			const float maxZ = center.z + extend.z;

			mParticles[0]->SetPosition({ minX, minY, minZ }); // Bottom left back
			mParticles[1]->SetPosition({ minX, minY, maxZ }); // Bottom left front
			mParticles[2]->SetPosition({ maxX, minY, maxZ }); // Bottom right back
			mParticles[3]->SetPosition({ maxX, minY, minZ }); // Bottom right front
			mParticles[4]->SetPosition({ minX, maxY, minZ }); // Top left back
			mParticles[5]->SetPosition({ minX, maxY, maxZ }); // Top left front
			mParticles[6]->SetPosition({ maxX, maxY, maxZ }); // Top right back
			mParticles[7]->SetPosition({ maxX, maxY, minZ }); // Top right front

			for (int i = 0; i < 8; i++)
			{
				mParticles[i]->SetVelocity({ RandomFloat(-0.05f,0.01f) ,RandomFloat(-0.1f,0.5f),RandomFloat(-0.05f,0.05f) });
			}

			auto c1 = new Physics::Spring(mParticles[0], mParticles[1]);
			auto c2 = new Physics::Spring(mParticles[1], mParticles[2]);
			auto c3 = new Physics::Spring(mParticles[2], mParticles[3]);
			auto c4 = new Physics::Spring(mParticles[3], mParticles[0]);

			auto c5 = new Physics::Spring(mParticles[4], mParticles[5]);
			auto c6 = new Physics::Spring(mParticles[5], mParticles[6]);
			auto c7 = new Physics::Spring(mParticles[6], mParticles[7]);
			auto c8 = new Physics::Spring(mParticles[7], mParticles[4]);

			auto c9 = new Physics::Spring(mParticles[0], mParticles[4]);
			auto c10 = new Physics::Spring(mParticles[1], mParticles[5]);
			auto c11 = new Physics::Spring(mParticles[2], mParticles[6]);
			auto c12 = new Physics::Spring(mParticles[3], mParticles[7]);

			auto c13 = new Physics::Spring(mParticles[0], mParticles[6]);
			auto c15 = new Physics::Spring(mParticles[1], mParticles[7]);
			auto c14 = new Physics::Spring(mParticles[2], mParticles[4]);
			auto c16 = new Physics::Spring(mParticles[3], mParticles[5]);

			mPhysicsWorld.AddConstraint(c1);
			mPhysicsWorld.AddConstraint(c2);
			mPhysicsWorld.AddConstraint(c3);
			mPhysicsWorld.AddConstraint(c4);

			mPhysicsWorld.AddConstraint(c5);
			mPhysicsWorld.AddConstraint(c6);
			mPhysicsWorld.AddConstraint(c7);
			mPhysicsWorld.AddConstraint(c8);

			mPhysicsWorld.AddConstraint(c9);
			mPhysicsWorld.AddConstraint(c10);
			mPhysicsWorld.AddConstraint(c11);
			mPhysicsWorld.AddConstraint(c12);

			mPhysicsWorld.AddConstraint(c13);
			mPhysicsWorld.AddConstraint(c14);
			mPhysicsWorld.AddConstraint(c15);
			mPhysicsWorld.AddConstraint(c16);
			mParticles.clear();
		}
	}
	if (ImGui::Button("Ball and Chain"))
	{
		mParticles.clear();
		usingClothTexture = false;
		int stringLength = 13;
		mPhysicsWorld.Clear(true);
		mParticles.push_back(new Physics::Particle());
		mParticles.back()->SetPosition({ 0.0f , 15.0f  , 0.0f });
		mParticles.back()->radius = 0.1f;
		auto c1 = new Physics::Fixed(mParticles[0]);
		mPhysicsWorld.AddConstraint(c1);
		for (int i = 1; i < stringLength; i++)
		{
			mParticles.push_back(new Physics::Particle());
			mParticles.back()->SetPosition({ 0.0f , 15.0f  , 0.0f });
			mParticles.back()->SetVelocity({ RandomFloat(-0.05f,0.01f) ,RandomFloat(-0.1f,0.5f),RandomFloat(-0.05f,0.05f) });
			mParticles.back()->radius = 0.1f;

			auto c2 = new Physics::Spring(mParticles[i - 1], mParticles[i], 1.0f);
			mPhysicsWorld.AddConstraint(c2);
		}

		mParticles.push_back(new Physics::Particle());
		mParticles.back()->SetPosition({ 0.0f , 15.0f  , 0.0f });
		mParticles.back()->SetVelocity({ RandomFloat(-0.05f,0.01f) , RandomFloat(-0.1f,1.5f),RandomFloat(-0.05f,0.05f) });
		mParticles.back()->radius = 1.0f;

		auto c3 = new Physics::Spring(mParticles[stringLength - 1], mParticles[stringLength], 1.0f);
		mPhysicsWorld.AddConstraint(c3);

		for (size_t i = 0; i < mParticles.size(); i++)
		{
			mPhysicsWorld.AddParticles(mParticles[i]);
		}

		mParticles.clear();
	}
	if (ImGui::Button("Cloth"))
	{
		mParticles.clear();
		usingClothTexture = false;
		mPhysicsWorld.Clear(true);
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				mParticles.push_back(new Physics::Particle());
				mParticles.back()->SetPosition({ -0.5f*width + static_cast<float>(x) , 1.5f* height - static_cast<float>(y)  , 0.0f });
				mParticles.back()->SetVelocity({ RandomFloat(-0.05f,0.01f) ,RandomFloat(-0.1f,0.5f),RandomFloat(-0.05f,0.05f) });
				mParticles.back()->radius = 0.1f;
				mParticles.back()->bounce = 0.3f;
				mPhysicsWorld.AddParticles(mParticles.back());
			}
		}

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				if (y == 0 && (x == 0 || x == static_cast<int>(width*0.5f) || x == width - 1))
				{
					auto c1 = new Physics::Fixed(mParticles[GetIndex(x, y, width)]);
					mPhysicsWorld.AddConstraint(c1);
				}
				if (x + 1 < width)
				{
					auto c1 = new Physics::Spring(mParticles[GetIndex(x, y, width)], mParticles[GetIndex(x + 1, y, width)]);
					mPhysicsWorld.AddConstraint(c1);
				}
				if (y + 1 < height)
				{
					auto c2 = new Physics::Spring(mParticles[GetIndex(x, y, width)], mParticles[GetIndex(x, y + 1, width)]);
					mPhysicsWorld.AddConstraint(c2);
				}
			}
		}
		mParticles.clear();
	}
	if (ImGui::Button("Cloth with Texture"))
	{
		mParticles.clear();
		usingClothTexture = true;
		mPhysicsWorld.Clear(true);
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				mParticles.push_back(new Physics::Particle());
				mParticles.back()->SetPosition({ -0.5f*width + static_cast<float>(x) , 1.5f* height - static_cast<float>(y)  , 0.0f });
				mParticles.back()->SetVelocity({ RandomFloat(-0.05f,0.01f) ,RandomFloat(-0.1f,0.5f),RandomFloat(-0.05f,0.05f) });
				mParticles.back()->radius = 0.1f;
				mParticles.back()->bounce = 0.3f;
				mPhysicsWorld.AddParticles(mParticles.back());
			}
		}

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				if (y == 0 && (x == 0 || x == static_cast<int>(width*0.5f) || x == width - 1))
				{
					auto c1 = new Physics::Fixed(mParticles[GetIndex(x, y, width)]);
					mPhysicsWorld.AddConstraint(c1);
				}
				if (x + 1 < width)
				{
					auto c1 = new Physics::Spring(mParticles[GetIndex(x, y, width)], mParticles[GetIndex(x + 1, y, width)]);
					mPhysicsWorld.AddConstraint(c1);
				}
				if (y + 1 < height)
				{
					auto c2 = new Physics::Spring(mParticles[GetIndex(x, y, width)], mParticles[GetIndex(x, y + 1, width)]);
					mPhysicsWorld.AddConstraint(c2);
				}
			}
		}
	}
	if (ImGui::Button("Clear"))
	{
		mPhysicsWorld.Clear(true);
	}

	static bool showParticles = true;
	if(ImGui::Checkbox("Show Particles", &showParticles))
	{
		mPhysicsWorld.ShowParticles(showParticles);
	}
	ImGui::End();
}

void GameState::DrawScene()
{
	auto matView = mCamera.GetViewMatrix();
	auto matProj = mCamera.GetPerspectiveMatrix();


	if (usingClothTexture)
	{
		auto context = GraphicsSystem::Get()->GetContext();
		mConstantBuffer.BindVS();

		mVertexShader.Bind();
		mPixelShader.Bind();
		mSampler.BindPS();
		mTexture.BindPS();

		auto matWVP = Transpose(matView * matProj);

		mConstantBuffer.Update(&matWVP);
		mMeshBuffer.Update(mMesh.vertices.data(), static_cast<uint32_t>(mMesh.vertices.size()));
		mMeshBuffer.Draw();
	}
	else
		mPhysicsWorld.DebugDraw();

	SimpleDraw::Render(mCamera);
}


// For Homework:
// Add the following
// - Tetrahedron
// - Cubes (how many constraints do you need)
// - Ball and Chain
// - Cloth
// - And if you are up for the challenge, build a figure (may need more constraints)
// - Build whatever you want