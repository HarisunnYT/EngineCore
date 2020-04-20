#include "..\Header Files\Plane.h"

void Plane::Draw()
{
	if (meshRenderer->GetTexture() != nullptr)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, meshRenderer->GetTexture()->texID);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vector3 pos = meshRenderer->entity->transform->position;
	glTranslatef(pos.x, pos.y, pos.z);

	Vector3 scale = meshRenderer->entity->transform->scale;
	glScalef(scale.x, scale.y, scale.z);

	Vector3 eulerRotation = meshRenderer->entity->transform->eulerRotation;
	glRotatef(eulerRotation.x, 1, 0, 0);
	glRotatef(eulerRotation.y, 0, 1, 0);
	glRotatef(eulerRotation.z, 0, 0, 1);

	glBegin(GL_QUADS);

	glNormal3f(0, 1, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);

	glEnd();

	glFlush();
}
