#include "GameState.h"
#include<GL/gl.h>
#include<GL/glu.h>

using namespace Angazi;
using namespace Angazi::Graphics;
using namespace Angazi::Input;
using namespace Angazi::Math;


void GameState::Initialize()
{

}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{

}

void GameState::Render()
{
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.5f, -0.5f);
	glVertex2f( 0.0f,  0.5f);
	glVertex2f( 0.5f, -0.5f);
	glEnd();
}