#include "..\Header Files\FBXMesh.h"

MeshFBX::MeshFBX(string name)
{
	verticies = nullptr;
	normals = nullptr;
	uvs = nullptr;

	numVerticies = 0;
}

MeshFBX::~MeshFBX()
{
	DeleteObjects();
}

bool MeshFBX::DrawMesh(Vector3* transformedVerts)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	MaterialFBX* lastMaterial = nullptr;
	for (int i = 0; i < (int)faces.size(); i++)
	{
		FaceFBX* face = faces[i];

		MaterialFBX* material = face->material;
		if (material != lastMaterial)
		{
			material->Kd[3] = material->alpha;
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*)material->Ka);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*)material->Kd);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*)material->Ks);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, (GLfloat*)material->Ke);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->shininess);

			if (material->diffuseMap != nullptr)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, material->diffuseMap->texID);
			}
			else
			{
				glDisable(GL_TEXTURE_2D);
			}

			lastMaterial = material;
		}

		glBegin(GL_TRIANGLES);

		if (transformedVerts != nullptr)
		{
			for (int v = 0; v < 3; v++)
			{
				if (face->UVs[v] != -1)
					glTexCoord2f(uvs[face->UVs[v]].x, uvs[face->UVs[v]].y);

				glNormal3d(normals[face->normals[v]].x, 
						   normals[face->normals[v]].y, 
					       normals[face->normals[v]].z);
				glVertex3d(transformedVerts[face->verticies[v]].x, 
						   transformedVerts[face->verticies[v]].y, 
					       transformedVerts[face->verticies[v]].z);
			}
		}
		else
		{
			for (int v = 0; v < 3; v++)
			{
				if (face->UVs[v] != -1)
					glTexCoord2f(uvs[face->UVs[v]].x, uvs[face->UVs[v]].y);

				glNormal3d(normals[face->normals[v]].x,
						   normals[face->normals[v]].y,
						   normals[face->normals[v]].z);
				glVertex3d(verticies[face->verticies[v]].x,
						   verticies[face->verticies[v]].y,
					       verticies[face->verticies[v]].z);
			}
		}

		glEnd();
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	return true;
}

vector<FaceFBX*>& MeshFBX::GetFaces(void)
{
	return faces;
}

Vector3* MeshFBX::GetVerticies(void)
{
	return verticies;
}

Vector3* MeshFBX::GetNormals(void)
{
	return normals;
}

Vector3* MeshFBX::GetUVs(void)
{
	return uvs;
}

void MeshFBX::AddFace(int v1, int v2, int v3, int n1, int n2, int n3, int uv1, int uv2, int uv3, MaterialFBX* material)
{
	FaceFBX* face = new FaceFBX(v1, v2, v3, n1, n2, n3, uv1, uv2, uv3, verticies);
	face->material = material;
	faces.push_back(face);
}

void MeshFBX::AddFace(FaceFBX* face)
{
	faces.push_back(face);
}

void MeshFBX::DeleteObjects(void)
{
	delete[] verticies;
	delete[] normals;
	delete[] uvs;

	for (int i = 0; i < (int)faces.size(); i++)
		delete faces[i];

	faces.clear();
}
