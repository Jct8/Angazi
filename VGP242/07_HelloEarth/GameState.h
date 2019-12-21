#pragma once
#include <Angazi/Inc/Angazi.h>

class GameState : public Angazi::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;

private:
	Angazi::Graphics::Camera mCamera;

	Angazi::Graphics::MeshPX mMeshPlane;
	Angazi::Graphics::MeshPX mMeshSphere;

	Angazi::Graphics::MeshBuffer mMeshBufferPlane;
	Angazi::Graphics::MeshBuffer mMeshBufferSphere;

	Angazi::Graphics::VertexShader mVertexShader;
	Angazi::Graphics::PixelShader mPixelShader;
	Angazi::Graphics::ConstantBuffer mConstantBuffer;

	Angazi::Graphics::Sampler mSampler;
	Angazi::Graphics::Texture mTexture;

	float mRotation = 0.0f;
};

//Homework
	// Update HelloTexuring to use a MeshPC data to draw texture mapped cubes
	// You will need to add sampler and Texture classes provided
	// You will need to use DoTexturing.fx shaders
	// Add a new class called Graphics::MeshBuilder with the following functions

	//namespace Angazi::Graphics
	/*{
		class Meshbuilder
		{
		public:
			static MeshPX CreatePlanePX();
			static MeshPX CreateCylinderPX();
			static MeshPX CreateSpherePX(float radius,int rings = 16, int slices = 16);
		};
	}*/

	//This will allow you ti create a mesh easily by doing
	//	auto mesh = MeshBuilder::CreateSpherePX(...);

	// Add HelloEarth to test a texture mapped sphere using Earth Texture

	// A plane:
	/*for (int y = 0; y < height; y++)
	{
		for (int i = 0; i < width; i++)
		{
			vertices.push_back({x,y,0.0f}...);
		}
	}*/

	// A cylinder:
	/*for (int y = 0; y < height; y++)
	{
		for (int theta = 0; theta < TwoPi; theta+= ...)
		{
			vertices.push_back({sin(theta),y,cos(theta)}...);
		}
	}*/

	// A sphere:
	/*for (int phi = 0; phi < Pi; phi+=)
	{
		for (int theta = 0; theta < TwoPi; theta+= ...)
		{
			vertices.push_back({sin(theta) * r ,y,cos(theta)*r}...);
		}
	}*/