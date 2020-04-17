#include "..\Header Files\Model.h"

#include <fstream>
#include <iostream>
#include <sstream>

using std::ifstream;

Model::Model(string fn)
{
	objectLoaded = false;

	if (fn != "")
	{
		LoadObject(fn);
	}

	displayList = 0;
}

Model::~Model()
{
	DeleteObjects();
}

void Model::DrawModel(void)
{
	if (!objectLoaded)
		return;

	if (displayList != 0)
	{
		glCallList(displayList);
		return;
	}

	displayList = glGenLists(1);

	glNewList(displayList, GL_COMPILE_AND_EXECUTE);

	DrawObject(false);
	DrawObject(true);

	glEndList();
}

void Model::DrawObject(bool transparency)
{
	Material* lastMaterial = nullptr;

	for (int i = 0; i < (int)objects.size(); i++)
	{
		GroupObject* object = objects[i];

		for (int n = 0; n < (int)object->faces.size(); n++)
		{
			Face* face = object->faces[n];

			Material* material = face->material;

			if (material != lastMaterial)
			{
				if (transparency == false && material->alpha < 1.0f)
				{
					continue;
				}

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

			DrawFace(*face);
		}
	}
}

void Model::DrawFace(Face& face)
{
	if ((int)face.numVertices <= 3)
		glBegin(GL_TRIANGLES);
	else
		glBegin(GL_POLYGON);

	for (int v = 0; v < (int)face.numVertices; v++)
	{
		if ((int)face.numUVWs > v && face.UVWs != nullptr)
			glTexCoord2f(face.UVWs[v]->x, face.UVWs[v]->y);
		if ((int)face.numNormals > v && face.normals != nullptr)
			glNormal3d(face.normals[v]->x, face.normals[v]->y, face.normals[v]->z);
		if ((int)face.numVertices > v && face.verticies != nullptr)
			glVertex3d(face.verticies[v]->x, face.verticies[v]->y, face.verticies[v]->z);
	}

	glEnd();
}

void Model::DeleteObjects(void)
{
	for (int m = 0; m < (int)materials.size(); m++)
	{
		if (materials[m]->ambientMap != nullptr)
			delete materials[m]->ambientMap;
		if (materials[m]->diffuseMap != nullptr)
			delete materials[m]->diffuseMap;
		if (materials[m]->specularMap != nullptr)
			delete materials[m]->specularMap;
		if (materials[m]->emissionMap != nullptr)
			delete materials[m]->emissionMap;
		if (materials[m]->shininessMap != nullptr)
			delete materials[m]->shininessMap;
		if (materials[m]->transparencyMap != nullptr)
			delete materials[m]->transparencyMap;
		if (materials[m]->bumpMap != nullptr)
			delete materials[m]->bumpMap;

		delete materials[m];
	}

	for (int m = 0; m < (int)verticies.size(); m++)
		delete verticies[m];

	for (int m = 0; m < (int)normals.size(); m++)
		delete normals[m];

	for (int m = 0; m < (int)UVWs.size(); m++)
		delete UVWs[m];

	for (int i = 0; i < (int)objects.size(); i++)
	{
		GroupObject* object = objects[i];

		for (int f = 0; f < (int)object->faces.size(); f++)
		{
			delete object->faces[f]->verticies;
			delete object->faces[f]->normals;
			delete object->faces[f]->UVWs;

			delete object->faces[f];
		}
	}

	UVWs.clear();
	normals.clear();
	verticies.clear();
	objects.clear();
	materials.clear();
}

bool Model::LoadObject(string fn)
{
	fileName = fn;
	std::ifstream istr(fileName.data());

	if (!istr)
	{
		return false;
	}

	DeleteObjects();

	displayList = 0;
	objectLoaded = false;

	GroupObject* defaultObject = new GroupObject();
	GroupObject* currentObject = defaultObject;

	objects.push_back(defaultObject);

	char path[256];
	strcpy_s(path, fileName.data());

	for (int i = (int)fileName.length(); path[i] != '\\' && path[i] != '/'; i--)
	{
		path[i] = 0;
	}

	Material* currentMaterial = nullptr;
	char line[256];

	while (istr.getline(line, 256))
	{
		istringstream newLine(line, std::istringstream::in);

		string firstWord;
		newLine >> firstWord;

		if (firstWord == "#") //comment
		{
		}
		else if (firstWord == "mtllib") //material
		{
			string materialFileName;

			while (newLine >> materialFileName)
			{
				LoadMaterials(materialFileName);
			}
		}
		else if (firstWord == "usemtl") //use material
		{
			string materialName;
			newLine >> materialName;

			for (int i = 0; i < (int)materials.size(); i++)
			{
				if (materials[i]->name == materialName)
				{
					currentMaterial = materials[i];
					break;
				}
			}
		}
		else if (firstWord == "v") //vertex
		{
			Vector3* vertex = new Vector3;
			newLine >> vertex->x >> vertex->y >> vertex->z;
			verticies.push_back(vertex);
		}
		else if (firstWord == "vt") //texture coord
		{
			Vector3* uvw = new Vector3;
			newLine >> uvw->x >> uvw->y >> uvw->z;
			UVWs.push_back(uvw);
		}
		else if (firstWord == "vn") //normal
		{
			Vector3* normal = new Vector3;
			newLine >> normal->x >> normal->y >> normal->z;
			normals.push_back(normal);
		}
		else if (firstWord == "g") //group
		{
			string objectName;
			newLine >> objectName;

			if (objectName == "default")
			{
				currentObject = defaultObject;
			}
			else
			{
				GroupObject* object = new GroupObject;
				object->objectName = objectName;
				newLine >> object->groupName;

				currentObject = object;
				objects.push_back(object);
			}
		}
		else if (firstWord == "f") //face
		{
			Face* newFace = new Face;
			newFace->material = currentMaterial;

			currentObject->faces.push_back(newFace);

			vector<Vector3*> tempVertices;
			vector<Vector3*> tempNormals;
			vector<Vector3*> tempUVWs;

			while (!newLine.eof())
			{
				int normal = 0;
				int vertex = 0;
				int uvw = 0;

				bool noUV = false;
				char temp;

				if ((int)verticies.size() > 0)
				{
					newLine >> vertex;

					while (newLine.get(temp))
					{
						if (temp == '/' || temp == ' ')
							break;
					}

					newLine.get(temp);

					if (temp == '/')
					{
						noUV = true;
					}
					else
					{
						newLine.unget();
					}

					tempVertices.push_back(verticies[--vertex]);
				}

				if ((int)UVWs.size() > 0 && noUV == false)
				{
					newLine >> uvw;
					while (newLine.get(temp))
					{
						if (temp == '/' || temp == ' ')
							break;
					}

					tempUVWs.push_back(UVWs[--uvw]);
				}

				if ((int)normals.size() > 0)
				{
					newLine >> normal;
					while (newLine.get(temp))
					{
						if (temp == '/' || temp == ' ')
							break;
					}

					tempNormals.push_back(normals[--normal]);
				}
			}

			newFace->numVertices = (int)tempVertices.size();
			newFace->numNormals = (int)tempNormals.size();
			newFace->numUVWs = (int)tempUVWs.size();

			newFace->verticies = new Vector3 * [newFace->numVertices];
			newFace->normals = new Vector3 * [newFace->numNormals];
			newFace->UVWs = new Vector3 * [newFace->numUVWs];

			for (int v = 0; v < newFace->numVertices; v++)
			{
				newFace->verticies[v] = tempVertices[v];
			}
			for (int v = 0; v < newFace->numNormals; v++)
			{
				newFace->normals[v] = tempNormals[v];
			}
			for (int v = 0; v < newFace->numUVWs; v++)
			{
				newFace->UVWs[v] = tempUVWs[v];
			}

			for (int v = 0; v < newFace->numVertices; v++)
			{
				newFace->faceCenter += (*tempVertices[v]);
			}

			newFace->faceCenter /= (float)newFace->numVertices;

			if (newFace->numVertices >= 3)
			{
				Vector3 vector1 = ((*newFace->verticies[0]) - (*newFace->verticies[1])).Normalised();
				Vector3 vector2 = ((*newFace->verticies[0]) - (*newFace->verticies[2])).Normalised();
				newFace->faceNormal = vector1 * vector2;
			}
		}
	}

	float xMin = 0, xMax = 0;
	float yMin = 0, yMax = 0;
	float zMin = 0, zMax = 0;

	center = Vector3::Zero();

	for (int n = 0; n < (int)verticies.size(); n++)
	{
		if (n == 0)
		{
			xMin = xMax = verticies[n]->x;
			yMin = yMax = verticies[n]->y;
			zMin = zMax = verticies[n]->z;
			continue;
		}

		if (verticies[n]->x < xMin)
			xMin = verticies[n]->x;
		if (verticies[n]->x > xMax)
			xMax = verticies[n]->x;
		if (verticies[n]->y < yMin)
			yMin = verticies[n]->y;
		if (verticies[n]->y > yMax)
			yMax = verticies[n]->y;
		if (verticies[n]->z < zMin)
			zMin = verticies[n]->z;
		if (verticies[n]->z > zMax)
			zMax = verticies[n]->z;

		center += *verticies[n];
	}

	center /= (float)verticies.size();

	boundingPoints[0] = Vector3(xMax, yMin, zMin);
	boundingPoints[1] = Vector3(xMin, yMax, zMin);
	boundingPoints[2] = Vector3(xMin, yMin, zMax);

	boundingPoints[3] = Vector3(xMin, yMax, zMax);
	boundingPoints[4] = Vector3(xMax, yMax, zMin);
	boundingPoints[5] = Vector3(xMax, yMin, zMax);

	boundingPoints[6] = Vector3(xMin, yMin, zMin);
	boundingPoints[7] = Vector3(xMax, yMax, zMax);

	radius = (Vector3(xMax, yMax, zMax) - Vector3(xMin, yMin, zMin)).Magnitude() / 2.0f;

	objectLoaded = true;

	return true;
}

void Model::LoadMaterials(string fn)
{
	ifstream istr(fn.data());

	if (!istr)
		return;

	char path[256];
	strcpy_s(path, fn.data());

	for (int i = (int)fileName.length(); path[i] != '\\' && path[i] != '/'; i--)
	{
		path[i] = 0;
	}

	Material* material = nullptr;

	char line[256];

	while (istr.getline(line, 256))
	{
		istringstream newLine(line, istringstream::in);

		string firstWord;
		newLine >> firstWord;

		if (firstWord == "newmtl")
		{
			material = new Material;
			materials.push_back(material);

			newLine >> material->name;
		}
		else if (firstWord == "illum") 
		{
			newLine >> material->illum;
		}
		else if (firstWord == "Ka") 
		{
			newLine >> material->Ka[0] >> material->Ka[1] >> material->Ka[2];
		}
		else if (firstWord == "Kd") 
		{
			newLine >> material->Kd[0] >> material->Kd[1] >> material->Kd[2];
		}
		else if (firstWord == "Ks")
		{
			newLine >> material->Ks[0] >> material->Ks[1] >> material->Ks[2];
		}
		else if (firstWord == "Ke")
		{
			newLine >> material->Ke[0] >> material->Ke[1] >> material->Ke[2];
		}
		else if (firstWord == "Ns")
		{
			newLine >> material->shininess;
		}
		else if (firstWord == "d" || firstWord == "Tr")
		{
			newLine >> material->alpha;
		}
		else if (firstWord == "Tf")
		{
			Vector3 color;
			newLine >> color.x >> color.y >> color.z;

			material->alpha = (color.x + color.y + color.z) / 3.0f;
		}
		else if (firstWord == "map_Ka")
		{
			string fileName;
			newLine >> fileName;
			Texture* map = new Texture(string(path) + fileName);
			material->ambientMap = map;
		}
		else if (firstWord == "map_Kd")
		{
			string fileName;
			newLine >> fileName;
			Texture* map = new Texture(string(path) + fileName);
			material->diffuseMap = map;
		}
		else if (firstWord == "map_Ks")
		{
			string fileName;
			newLine >> fileName;
			Texture* map = new Texture(string(path) + fileName);
			material->specularMap = map;
		}
		else if (firstWord == "map_Ke")
		{
			string fileName;
			newLine >> fileName;
			Texture* map = new Texture(string(path) + fileName);
			material->emissionMap = map;
		}
		else if (firstWord == "map_Ns")
		{
			string fileName;
			newLine >> fileName;
			Texture* map = new Texture(string(path) + fileName);
			material->shininessMap = map;
		}
		else if (firstWord == "map_d")
		{
			string fileName;
			newLine >> fileName;
			Texture* map = new Texture(string(path) + fileName);
			material->transparencyMap = map;
		}
		else if (firstWord == "map_Bump")
		{
			string fileName;
			newLine >> fileName;
			Texture* map = new Texture(string(path) + fileName);
			material->bumpMap = map;
		}
	}
}

float Model::GetRadius(void)
{
	return radius;
}

Vector3 Model::GetCenter(void)
{
	return center;
}

string Model::GetPath()
{
	return fileName;
}
