#include "GameScreenLevel1.h"
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <string>				
#include <sstream>					
#include "../gl/glut.h"
#include "Constants.h"
#include "Camera.h"
#include "Texture2D.h"


using namespace::std;

//----------------------------------------------------------------------------

GameScreenLevel1::GameScreenLevel1() : GameScreen()
{
	srand(time(NULL));

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	gluPerspective(60.0f,aspect,0.1f,1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	newcamera = Camera::GetInstance();

	Texture2D* SpinningTopTexture = new Texture2D();

	SpinningTopTexture->Load("LightWood.raw", 512, 512);
	glBindTexture(GL_TEXTURE_2D, SpinningTopTexture->GetID());
	SpinningTop = new Object3DS(Vector3D(-10.0f, -30.0f, -100.0f), "SpinningTop.3ds", SpinningTopTexture->GetID());

	Texture2D* SpinningTop2Texture = new Texture2D();

	SpinningTop2Texture->Load("RedWood.raw", 512, 512);
	glBindTexture(GL_TEXTURE_2D, SpinningTop2Texture->GetID());
	SpinningTop2 = new Object3DS(Vector3D(40.0f, -30.0f, -100.0f), "SpinningTop2.3ds", SpinningTop2Texture->GetID());

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

//----------------------------------------------------------------------------

GameScreenLevel1::~GameScreenLevel1()
{	
}

//----------------------------------------------------------------------------

void GameScreenLevel1::SetLight()
{
	lighting light = 
	{
		{ 0.5f, 0.5f, 0.5f, 1.0f },
		{ 0.7f, 0.7f, 0.7f, 1.0f },
		{ 0.5f, 0.5f, 0.5f, 1.0f }
	};

	float light_pos[] = { 1.0f, 1.0f, 1.0f, 0.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light.ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light.diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light.specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
}

//----------------------------------------------------------------------------

void GameScreenLevel1::SetMaterial()
{
	material material =
	{
		{ 0.50f, 0.50f, 0.50f, 1.0f },
		{ 0.50f, 0.50f, 0.50f, 1.0f },
		{ 0.5f, 0.5f, 0.5f, 1.0f },
		0.0f
	};

	glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
	glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);
}

//------------------------------------------------------------------------------

void GameScreenLevel1::OutputLine(float x, float y, string text)
{
	glRasterPos2f(x, y);												
	for (int i = 0; i < text.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[i]);
	}
}

//----------------------------------------------------------------------------

void GameScreenLevel1::Render()
{
	//Clear the screen.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Camera::GetInstance()->Render();
	SpinningTop->Render();
	SpinningTop2->Render();

	SetLight();
	SetMaterial();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 100, 0, 100); 
	OutputLine(5, 95, "Spinning Top.");
	OutputLine(5, 92, "Camera can be adjusted using the arrow keys and WASD.");
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}


//----------------------------------------------------------------------------

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	Camera::GetInstance()->Update(deltaTime, e);

	mCurrentTime += deltaTime;
	SpinningTop->Update(deltaTime);
	SpinningTop2->Update(deltaTime);
}

//----------------------------------------------------------------------------