#include "3DSObject.h"
#include "../gl/glut.h"
#include "3DSLoader.h"
#include "GameScreenLevel1.h"

Object3DS::Object3DS(Vector3D startPosition, string modelFileName, int texID)
{
	mPosition = startPosition;															
	std::strcpy(fileName, modelFileName.c_str());										
	loadModel();
	object.id_texture = texID;
	loadTexture();
}

void Object3DS::loadModel()
{
	if (fileName[0] != '---')
		Load3DS(&object, fileName);
}

void Object3DS::loadTexture()
{
	
}

void Object3DS::Update(float deltaTime)
{

}

void Object3DS::Render()
{
	glPushMatrix();
	glTranslatef(mPosition.x, mPosition.y, mPosition.z);
	glRotatef(-90.0f, 180.0f, 0.0f, 0.0f);
	glScalef(1.5f, 1.5f, 1.5f);

	glBindTexture(GL_TEXTURE_2D, object.id_texture);									

	glBegin(GL_TRIANGLES);
	for (int l_index = 0; l_index < object.triangles_qty; l_index++)
	{
		glTexCoord2f
			(object.mapcoord[object.triangles[l_index].a].u,							
			 object.mapcoord[object.triangles[l_index].a].v);
		glVertex3f
			(object.vertex[object.triangles[l_index].a].x,								
			 object.vertex[object.triangles[l_index].a].y,
			 object.vertex[object.triangles[l_index].a].z);

		glTexCoord2f
			(object.mapcoord[object.triangles[l_index].b].u,							
		   object.mapcoord[object.triangles[l_index].b].v);
		glVertex3f
			(object.vertex[object.triangles[l_index].b].x,								
			 object.vertex[object.triangles[l_index].b].y,
			 object.vertex[object.triangles[l_index].b].z);

		glTexCoord2f
			(object.mapcoord[object.triangles[l_index].c].u,							
		   object.mapcoord[object.triangles[l_index].c].v);
		glVertex3f
			(object.vertex[object.triangles[l_index].c].x,								
			 object.vertex[object.triangles[l_index].c].y,
			 object.vertex[object.triangles[l_index].c].z);
	}
	glEnd();
	glPopMatrix();
}