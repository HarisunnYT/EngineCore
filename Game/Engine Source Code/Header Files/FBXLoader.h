#pragma once

#ifndef FBXLOADER_H
#define FBXLOADER_H

#include "FBXMesh.h"
#include "Vector3.h"
#include "Texture.h"
#include "Light.h"

#include "3rdPartyLibs/Includes/FBX/fbxsdk.h"

#include <string>

struct Object
{
	std::string			name;
	vector<Object*>		children;

	MeshFBX				*mesh;
	double				*matrix;

	Object()
	{
		mesh = nullptr;
		matrix = nullptr;
	}

};

class FBXLoader
{
public:

	FBXLoader(std::string fileName = "");
	~FBXLoader();

	void DrawModel(long time);
	void DrawModel(Object* object, long time);

	bool LoadFBX(std::string fileName);
	void LoadContent(fbxsdk::FbxScene* scene, fbxsdk::FbxNode* node, vector<Object*> *objects);

private:

	void ComputeTransformation(fbxsdk::FbxMatrix& globalPosition,
							   fbxsdk::FbxMesh* pMesh,
							   fbxsdk::FbxVector4* vertexArray,
							   MeshFBX* mesh,
							   fbxsdk::FbxTime& time,
							   fbxsdk::FbxPose* pose);

	void LoadSupportedTextures(fbxsdk::FbxScene* scene);
	void LoadSupportedTexturesRecursive(fbxsdk::FbxNode *node);


	vector<Object*>			objects;
	vector<MaterialFBX*>	materials;

	std::string				fileName;
	bool					objectLoaded;

};

#endif