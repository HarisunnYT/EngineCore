#pragma once

#ifndef MODEL_H
#define MODEL_H

#include "Vector3.h"
#include "Texture.h"

#include "SDL_opengl.h"

#include <string>
#include <vector>

struct Material
{
	float Ka[4]; 
	float Kd[4];
	float Ks[4];
	float Ke[4];

	float shininess;
	float alpha;
	float illum;

	Texture* ambientMap;
	Texture* diffuseMap;
	Texture* specularMap;
	Texture* emissionMap;
	Texture* shininessMap;
	Texture* transparencyMap;
	Texture* bumpMap;

	std::string name;

	Material()
	{
		float defaultAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		float defaultDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float defaultSpecular[4] = { 0.0f, 0.0f, 0.0f, 1.0 };
		float defaultEmission[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		memcpy(Ka, defaultAmbient, sizeof(float) * 4);
		memcpy(Kd, defaultDiffuse, sizeof(float) * 4);
		memcpy(Ks, defaultSpecular, sizeof(float) * 4);
		memcpy(Ke, defaultEmission, sizeof(float) * 4);

		ambientMap = nullptr;
		diffuseMap = nullptr;
		specularMap = nullptr;
		emissionMap = nullptr;
		shininessMap = nullptr;
		transparencyMap = nullptr;
		bumpMap = nullptr;
	}
};

struct Face
{
	Vector3** verticies;
	int numVertices;

	Vector3** normals;
	int numNormals;

	Vector3** UVWs;
	int numUVWs;

	Material* material;

	Vector3 faceCenter;
	Vector3 faceNormal;

	Face()
	{
		verticies = nullptr;
		normals = nullptr;
		UVWs = nullptr; 
		material = nullptr;

		numVertices = 0;
		numNormals = 0;
		numUVWs = 0;
	}
};

struct GroupObject
{
	vector<Face*>	faces;

	string			objectName;
	string			groupName;
};

class Model
{
public:

	Model(string fileName = "");
	~Model();

	void		DrawModel(void);
	void		DrawObject(bool transparency = false);
	void		DrawFace(Face& face);

	void		DeleteObjects(void);
	bool		LoadObject(string fileName);
	void		LoadMaterials(string fileName);

	float		GetRadius(void);
	Vector3		GetCenter(void);
	string		GetPath();

private:

	vector<GroupObject*>	objects;

	vector<Vector3*>		verticies;
	vector<Vector3*>		normals;
	vector<Vector3*>		UVWs;

	vector<Material*>		materials;

	Vector3					boundingPoints[8];

	float					radius;
	Vector3					center;

	bool					objectLoaded;

	GLuint					displayList;

	string					fileName;

};

#endif