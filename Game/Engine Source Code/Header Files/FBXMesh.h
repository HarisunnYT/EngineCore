#pragma once

#ifndef FBXMESH_H
#define FBXMESH_H

#include "Texture.h"
#include "Vector3.h"

#include "3rdPartyLibs/Includes/FBX/fbxsdk.h"

#include <string>

struct MaterialFBX
{
	float Ka[4];
	float Kd[4];
	float Ks[4];
	float Ke[4];

	float shininess;
	float alpha;
	float illum;

	Texture* diffuseMap;

	std::string name;

	MaterialFBX()
	{
		float defaultAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		float defaultDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float defaultSpecular[4] = { 0.0f, 0.0f, 0.0f, 1.0 };
		float defaultEmission[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		memcpy(Ka, defaultAmbient, sizeof(float) * 4);
		memcpy(Kd, defaultDiffuse, sizeof(float) * 4);
		memcpy(Ks, defaultSpecular, sizeof(float) * 4);
		memcpy(Ke, defaultEmission, sizeof(float) * 4);

		diffuseMap = nullptr;

		shininess = 2.0f;
		alpha = 1.0f;
		illum = 1.0f;
	}
};

struct FaceFBX
{
	int verticies[3];
	int normals[3];
	int UVs[3];

	MaterialFBX* material;

	Vector3 faceCenter;
	Vector3 faceNormal;

	FaceFBX(int x, int y, int z,
		 int n1, int n2, int n3,
		 int uv1, int uv2, int uv3,
		 Vector3 *v)
	{
		verticies[0] = x;
		verticies[1] = y;
		verticies[2] = z;

		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;

		UVs[0] = uv1;
		UVs[1] = uv2;
		UVs[2] = uv3;

		for (int i = 0; i < 3; i++)
			faceCenter += v[verticies[i]];

		faceCenter /= 3.0f;
		faceNormal = (v[x] - v[y]).Normalise() * (v[x] - v[z]).Normalise();

		material = nullptr;
	}
};

struct Frame
{
	fbxsdk::FbxMatrix 	*bones;
	double				*weights;
};

class MeshFBX
{
public:

	MeshFBX(string name = "");
	~MeshFBX();

	bool DrawMesh(Vector3* transformedVerts);

private:

	vector<FaceFBX*>& GetFaces(void);
	Vector3* GetVerticies(void);
	Vector3* GetNormals(void);
	Vector3* GetUVs(void);

	void AddFace(int v1, int v2, int v3,
		int n1, int n2, int n3,
		int uv1, int uv2, int uv3,
		MaterialFBX* material);

	void AddFace(FaceFBX* face);

	void DeleteObjects(void);

	Vector3* verticies;
	Vector3* normals;
	Vector3* uvs;

	int					numVerticies;
	vector<FaceFBX*>	faces;

	string				name;

	float				frameStart;
	float				frameEnd;
	float				frame;

	vector<Frame*>		frames;

	friend class FBXLoader;
};

#endif