#include "..\Header Files\FBXLoader.h"
#include "GetPosition.h"

using namespace fbxsdk;

const float frameRate = 30.0f;

FBXLoader::FBXLoader(std::string fn)
{
	objectLoaded = false;
	fileName = fn;

	if (!fileName.empty())
	{
		LoadFBX(fileName);
	}
}

FBXLoader::~FBXLoader()
{
	for (int m = 0; m < (int)materials.size(); m++)
	{
		if (materials[m]->diffuseMap != nullptr)
		{
			delete materials[m]->diffuseMap;
		}

		delete materials[m];
	}

	for (int i = 0; i < (int)objects.size(); i++)
	{
		delete objects[i];
	}
}

void FBXLoader::DrawModel(long time)
{
	if (!objectLoaded)
		return;

	for (int i = 0; i < (int)objects.size(); i++)
	{
		Object* object = objects[i];
		DrawModel(object, time);
	}
}

void FBXLoader::DrawModel(Object* object, long time)
{
	static long lastTime = time;

	if (object == nullptr)
		return;

	MeshFBX* mesh = object->mesh;

	if (time != lastTime)
	{
		mesh->frame += (float)(time - lastTime) / 1000.0f * frameRate;

		if (mesh->frame > mesh->frameEnd - mesh->frameStart)
		{
			mesh->frame = 0.0f;
		}

		lastTime = time;
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	FbxMatrix* linkDeformation = mesh->frames[(int)mesh->frame]->bones;
	double   * linkWeight = mesh->frames[(int)mesh->frame]->weights;

	Vector3* transformedVerts = new Vector3[mesh->numVerticies];

	for (int i = 0; i < mesh->numVerticies; i++)
	{
		FbxVector4 vertex(mesh->verticies[i].x, mesh->verticies[i].y, mesh->verticies[i].z);
		FbxVector4 transformedVertex = vertex;

		double weight = linkWeight[i];

		if (weight != 0.0)
		{
			FbxMatrix matrix = linkDeformation[i];
			transformedVertex = matrix.MultNormalize(vertex);
			transformedVertex += vertex * (1.0 - weight);
		}

		transformedVerts[i] = Vector3((float)transformedVertex[0], (float)transformedVertex[1], (float)transformedVertex[2]);
	}

	object->mesh->DrawMesh(transformedVerts);

	delete [] transformedVerts;

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	for (int i = 0; i < (int)object->children.size(); i++)
	{
		DrawModel(object->children[i], time);
	}
}

bool FBXLoader::LoadFBX(std::string fileName)
{
	FbxManager* sdkManager;
	FbxScene* scene;

	sdkManager = FbxManager::Create();

	if (!sdkManager)
	{
		return false;
	}

	FbxImporter* importer = FbxImporter::Create(sdkManager, " ");
	importer->Initialize(fileName.c_str(), -1, sdkManager->GetIOSettings());

	scene = FbxScene::Create(sdkManager, "myScene");

	importer->Import(scene);
	importer->Destroy();

	FbxNode* node = scene->GetRootNode();

	LoadSupportedTextures(scene);

	if (node != nullptr)
	{
		for (int i = 0; i < node->GetChildCount(); i++)
		{
			LoadContent(scene, node->GetChild(i), &objects);
		}
	}

	objectLoaded = true;

	sdkManager->Destroy();

	return true;
}

void FBXLoader::LoadContent(fbxsdk::FbxScene* scene, fbxsdk::FbxNode* node, vector<Object*>* objects)
{
	Object* object = nullptr;
	vector<Object*> *nextList = objects;

	if (node->GetNodeAttribute() == nullptr)
	{
	}
	else
	{
		switch (node->GetNodeAttribute()->GetAttributeType())
		{
			case FbxNodeAttribute::eLight:
			{
				Light* light = nullptr;
				FbxLight* pLight = (FbxLight*)node->GetNodeAttribute();

				if (pLight->LightType.Get() == FbxLight::ePoint)
				{
					light = new Light(LIGHT_TYPE::LIGHT_POINT);
				}
				else if (pLight->LightType.Get() == FbxLight::eDirectional)
				{
					light = new Light(LIGHT_TYPE::LIGHT_DIRECTIONAL);
				}
				else if (pLight->LightType.Get() == FbxLight::eSpot)
				{
					light = new Light(LIGHT_TYPE::LIGHT_SPOT);
				}

				FbxMatrix direction = node->EvaluateGlobalTransform();
				float x = (float)direction.Get(1, 0);
				float y = (float)direction.Get(1, 1);
				float z = (float)direction.Get(1, 2);

				light->entity->transform->position = Vector3(x, y, z);

				FbxDouble intensity = pLight->Intensity.Get();

				FbxColor lightColor;
				lightColor.mRed = node->GetNodeAttribute()->Color.EvaluateValue(0)[0] * intensity;
				lightColor.mGreen = node->GetNodeAttribute()->Color.EvaluateValue(0)[1] * intensity;
				lightColor.mBlue = node->GetNodeAttribute()->Color.EvaluateValue(0)[2] * intensity;

				light->SetDiffuse((float)lightColor.mRed, (float)lightColor.mGreen, (float)lightColor.mBlue, 1.0f);
			}
			break;

			case FbxNodeAttribute::eMesh:
			{
				object = new Object();
				object->mesh = nullptr;

				object->matrix = new double[16];
				memcpy(object->matrix, (double*)node->EvaluateGlobalTransform(), sizeof(double) * 16);

				object->name = node->GetName();

				FbxMesh* pMesh = (FbxMesh*)node->GetNodeAttribute();
				MaterialFBX* material = nullptr;

				FbxSurfaceMaterial *lMaterial = nullptr;
				FbxTexture* lTexture = nullptr;

				FbxLayerElementMaterial* lMaterialLayer = pMesh->GetLayer(0)->GetMaterials();
				FbxLayerElementTexture* lTextureLayer = pMesh->GetLayer(0)->GetTextures(FbxLayerElement::EType::eTextureDiffuse);
				
				material = new MaterialFBX();
				
				if (lMaterialLayer != nullptr && lMaterialLayer->mDirectArray->GetCount() > 0)
				{
					FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(lMaterialLayer->mDirectArray->GetAt(0));

					material->Kd[0] = lambert->Diffuse.Get()[0];
					material->Kd[1] = lambert->Diffuse.Get()[1];
					material->Kd[2] = lambert->Diffuse.Get()[2];

					//material->Ks[0] = lambert->spe
					//material->Ks[1] = specularAnimCurveNode->GetCurve(1, 0)->GetValue();
					//material->Ks[2] = specularAnimCurveNode->GetCurve(2, 0)->GetValue();

					material->Ka[0] = lambert->Ambient.Get()[0];
					material->Ka[1] = lambert->Ambient.Get()[1];
					material->Ka[2] = lambert->Ambient.Get()[2];
				}

				if (lTextureLayer != nullptr && lTextureLayer->mDirectArray->GetCount() > 0)
				{
					lTexture = lTextureLayer->mDirectArray->GetAt(0);

					for (int i = 0; i < (int)Texture::textures.size(); i++)
					{
						if (Texture::textures[i]->name == lTexture->GetName())
						{
							material->diffuseMap = Texture::textures[i];
							break;
						}
					}
				}

				int polygonCount = pMesh->GetPolygonCount();
				FbxVector4* controlPoints = pMesh->GetControlPoints();
				FbxLayerElementNormal* normals = pMesh->GetLayer(0)->GetNormals(); //TRY LAYER 1 if this doesn't work
				FbxLayerElementUV* UVs = pMesh->GetLayer(0)->GetUVs();

				MeshFBX *mesh = new MeshFBX();
				object->mesh = mesh;

				mesh->verticies = new Vector3[pMesh->GetControlPointsCount()];
				mesh->normals = new Vector3[pMesh->GetControlPointsCount()];
				mesh->uvs = new Vector3[pMesh->GetTextureUVCount()];
				mesh->numVerticies = pMesh->GetControlPointsCount();

				for (int i = 0; i < mesh->numVerticies; i++)
				{
					mesh->verticies[i] = Vector3((float)controlPoints[i][0], (float)controlPoints[i][1], (float)controlPoints[i][2]);
					mesh->normals[i] = Vector3((float)normals->GetDirectArray().GetAt(i)[0], (float)normals->GetDirectArray().GetAt(i)[1], (float)normals->GetDirectArray().GetAt(i)[2]);
				}

				for (int i = 0; i < pMesh->GetTextureUVCount(); i++)
				{
					mesh->uvs[i] = Vector3((float)UVs->GetDirectArray().GetAt(i)[0], (float)UVs->GetDirectArray().GetAt(i)[1], (float)UVs->GetDirectArray().GetAt(i)[2]);
				}

				for (int i = 0; i < polygonCount; i++)
				{
					int polygonSize = pMesh->GetPolygonSize(i);

					for (int j = 0; j < polygonSize; j++)
					{
						int index1 = pMesh->GetPolygonVertex(i, 0);
						int index2 = pMesh->GetPolygonVertex(i, j);
						int index3 = pMesh->GetPolygonVertex(i, j + 1);

						int uv1 = pMesh->GetTextureUVIndex(i, 0);
						int uv2 = pMesh->GetTextureUVIndex(i, j);
						int uv3 = pMesh->GetTextureUVIndex(i, j + 1);

						mesh->AddFace(index1, index2, index3,
									  index1, index2, index3,
									  uv1, uv2, uv3, material);
					}
				}

				FbxArray<FbxString*> takeNameArray;
				scene->FillAnimStackNameArray(takeNameArray);

				int numTakes = takeNameArray.GetCount();
				FbxTime period;
				period.SetTime(0, 0, 0, 1, 0, scene->GetGlobalSettings().GetTimeMode());

				for (int t = 0; t < numTakes; t++)
				{
					FbxTime currentTime, start, stop;
					FbxTakeInfo* currentTakeInfo = scene->GetTakeInfo(*takeNameArray[t]);
					if (currentTakeInfo != nullptr)
					{
						scene->SetTakeInfo(*currentTakeInfo);
						start = currentTakeInfo->mLocalTimeSpan.GetStart();
						stop = currentTakeInfo->mLocalTimeSpan.GetStop();

						FbxTime timeDifference = stop - start;
						FbxDouble difference = timeDifference.GetSecondDouble();

						int numFrames = (int)(difference * frameRate);
						mesh->frameStart = (float)(start.GetSecondDouble() * frameRate);
						mesh->frameEnd = (float)(stop.GetSecondDouble() * frameRate);
						mesh->frame = mesh->frameStart;

						FbxPose* pose = scene->GetPose(0);
						for (int i = 0; i < numFrames; i++)
						{
							FbxMatrix globalPosition = GetGlobalPosition(pMesh->GetNode(), currentTime, pose);
							ComputeTransformation(globalPosition, pMesh, controlPoints, mesh, currentTime, pose);

							currentTime += period;
						}

						break;
					}
				}

				objects->push_back(object);
				nextList = &object->children;

				break;

			}
		}
	}

	for (int i = 0; i < node->GetChildCount(); i++)
	{
		LoadContent(scene, node->GetChild(i), nextList);
	}
}

void FBXLoader::ComputeClusterDeformation(FbxAMatrix& pGlobalPosition,
	FbxMesh* pMesh,
	FbxCluster* pCluster,
	FbxAMatrix& pVertexTransformMatrix,
	FbxTime pTime,
	FbxPose* pPose)
{
	FbxCluster::ELinkMode lClusterMode = pCluster->GetLinkMode();

	FbxAMatrix lReferenceGlobalInitPosition;
	FbxAMatrix lReferenceGlobalCurrentPosition;
	FbxAMatrix lAssociateGlobalInitPosition;
	FbxAMatrix lAssociateGlobalCurrentPosition;
	FbxAMatrix lClusterGlobalInitPosition;
	FbxAMatrix lClusterGlobalCurrentPosition;

	FbxAMatrix lReferenceGeometry;
	FbxAMatrix lAssociateGeometry;
	FbxAMatrix lClusterGeometry;

	FbxAMatrix lClusterRelativeInitPosition;
	FbxAMatrix lClusterRelativeCurrentPositionInverse;

	if (lClusterMode == FbxCluster::eAdditive && pCluster->GetAssociateModel())
	{
		pCluster->GetTransformAssociateModelMatrix(lAssociateGlobalInitPosition);
		// Geometric transform of the model
		lAssociateGeometry = GetGeometry(pCluster->GetAssociateModel());
		lAssociateGlobalInitPosition *= lAssociateGeometry;
		lAssociateGlobalCurrentPosition = GetGlobalPosition(pCluster->GetAssociateModel(), pTime, pPose);

		pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
		// Multiply lReferenceGlobalInitPosition by Geometric Transformation
		lReferenceGeometry = GetGeometry(pMesh->GetNode());
		lReferenceGlobalInitPosition *= lReferenceGeometry;
		lReferenceGlobalCurrentPosition = pGlobalPosition;

		// Get the link initial global position and the link current global position.
		pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
		// Multiply lClusterGlobalInitPosition by Geometric Transformation
		lClusterGeometry = GetGeometry(pCluster->GetLink());
		lClusterGlobalInitPosition *= lClusterGeometry;
		lClusterGlobalCurrentPosition = GetGlobalPosition(pCluster->GetLink(), pTime, pPose);

		// Compute the shift of the link relative to the reference.
		//ModelM-1 * AssoM * AssoGX-1 * LinkGX * LinkM-1*ModelM
		pVertexTransformMatrix = lReferenceGlobalInitPosition.Inverse() * lAssociateGlobalInitPosition * lAssociateGlobalCurrentPosition.Inverse() *
			lClusterGlobalCurrentPosition * lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;
	}
	else
	{
		pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
		lReferenceGlobalCurrentPosition = pGlobalPosition;
		// Multiply lReferenceGlobalInitPosition by Geometric Transformation
		lReferenceGeometry = GetGeometry(pMesh->GetNode());
		lReferenceGlobalInitPosition *= lReferenceGeometry;

		// Get the link initial global position and the link current global position.
		pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
		lClusterGlobalCurrentPosition = GetGlobalPosition(pCluster->GetLink(), pTime, pPose);

		// Compute the initial position of the link relative to the reference.
		lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;

		// Compute the current position of the link relative to the reference.
		lClusterRelativeCurrentPositionInverse = lReferenceGlobalCurrentPosition.Inverse() * lClusterGlobalCurrentPosition;

		// Compute the shift of the link relative to the reference.
		pVertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition;
	}
}

void FBXLoader::ComputeTransformation(fbxsdk::FbxMatrix& globalPosition, fbxsdk::FbxMesh* pMesh, fbxsdk::FbxVector4* vertexArray, MeshFBX* mesh, fbxsdk::FbxTime& time, fbxsdk::FbxPose* pose)
{
	// All the links must have the same link mode.
	FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();

	int lVertexCount = pMesh->GetControlPointsCount();
	FbxAMatrix* lClusterDeformation = new FbxAMatrix[lVertexCount];
	memset(lClusterDeformation, 0, lVertexCount * sizeof(FbxAMatrix));

	double* lClusterWeight = new double[lVertexCount];
	memset(lClusterWeight, 0, lVertexCount * sizeof(double));

	if (lClusterMode == FbxCluster::eAdditive)
	{
		for (int i = 0; i < lVertexCount; ++i)
		{
			lClusterDeformation[i].SetIdentity();
		}
	}

	// For all skins and all clusters, accumulate their deformation and weight
	// on each vertices and store them in lClusterDeformation and lClusterWeight.
	int lSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex)
	{
		FbxSkin* lSkinDeformer = (FbxSkin*)pMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin);

		int lClusterCount = lSkinDeformer->GetClusterCount();
		for (int lClusterIndex = 0; lClusterIndex < lClusterCount; ++lClusterIndex)
		{
			FbxCluster* lCluster = lSkinDeformer->GetCluster(lClusterIndex);
			if (!lCluster->GetLink())
				continue;

			FbxAMatrix lVertexTransformMatrix;
			ComputeClusterDeformation((FbxAMatrix&)globalPosition, pMesh, lCluster, lVertexTransformMatrix, time, pose);

			int lVertexIndexCount = lCluster->GetControlPointIndicesCount();
			for (int k = 0; k < lVertexIndexCount; ++k)
			{
				int lIndex = lCluster->GetControlPointIndices()[k];

				// Sometimes, the mesh can have less points than at the time of the skinning
				// because a smooth operator was active when skinning but has been deactivated during export.
				if (lIndex >= lVertexCount)
					continue;

				double lWeight = lCluster->GetControlPointWeights()[k];

				if (lWeight == 0.0)
				{
					continue;
				}

				// Compute the influence of the link on the vertex.
				FbxAMatrix lInfluence = lVertexTransformMatrix;
				MatrixScale(lInfluence, lWeight);

				if (lClusterMode == FbxCluster::eAdditive)
				{
					// Multiply with the product of the deformations on the vertex.
					MatrixAddToDiagonal(lInfluence, 1.0 - lWeight);
					lClusterDeformation[lIndex] = lInfluence * lClusterDeformation[lIndex];

					// Set the link to 1.0 just to know this vertex is influenced by a link.
					lClusterWeight[lIndex] = 1.0;
				}
				else // lLinkMode == FbxCluster::eNormalize || lLinkMode == FbxCluster::eTotalOne
				{
					// Add to the sum of the deformations on the vertex.
					MatrixAdd(lClusterDeformation[lIndex], lInfluence);

					// Add to the sum of weights to either normalize or complete the vertex.
					lClusterWeight[lIndex] += lWeight;
				}
			}//For each vertex			
		}//lClusterCount
	}

	Frame* frame = new Frame();
	frame->bones = (FbxMatrix*)lClusterDeformation;
	frame->weights = lClusterWeight;

	mesh->frames.push_back(frame);
}

void FBXLoader::MatrixScale(FbxAMatrix& pMatrix, double pValue)
{
	int i, j;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			pMatrix[i][j] *= pValue;
		}
	}
}

void FBXLoader::MatrixAdd(FbxAMatrix& pDstMatrix, FbxAMatrix& pSrcMatrix)
{
	int i, j;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			pDstMatrix[i][j] += pSrcMatrix[i][j];
		}
	}
}

void FBXLoader::MatrixAddToDiagonal(FbxAMatrix& pMatrix, double pValue)
{
	pMatrix[0][0] += pValue;
	pMatrix[1][1] += pValue;
	pMatrix[2][2] += pValue;
	pMatrix[3][3] += pValue;
}

void FBXLoader::LoadSupportedTextures(fbxsdk::FbxScene* scene)
{
	LoadSupportedTexturesRecursive(scene->GetRootNode());
}

void FBXLoader::LoadSupportedTexturesRecursive(fbxsdk::FbxNode* node)
{
	if (node != nullptr)
	{
		FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
		if (nodeAttribute != nullptr)
		{
			FbxLayerContainer* layerContainer = nullptr;
			switch (nodeAttribute->GetAttributeType())
			{
				case FbxNodeAttribute::EType::eNurbs:
					layerContainer = node->GetNurbs();
					break;

				case FbxNodeAttribute::EType::ePatch:
					layerContainer = node->GetPatch();
					break;

				case FbxNodeAttribute::EType::eMesh:
					layerContainer = node->GetMesh();
					break;
			}

			if (layerContainer != nullptr && layerContainer->GetLayer(0))
			{
				FbxLayerElementTexture* textureLayer = layerContainer->GetLayer(0)->GetTextures(FbxLayerElement::EType::eUnknown);
				if (textureLayer != nullptr)
				{
					int count = textureLayer->GetDirectArray().GetCount();

					for (int x = 0; x < count; x++)
					{
						FbxFileTexture* lTexture = static_cast<FbxFileTexture*>(textureLayer->GetDirectArray().GetAt(x));
						const char* textureFileName = lTexture->GetFileName();

						Texture* texture = nullptr;
						bool loadTexture = true;

						for (int i = 0; i < (int)Texture::textures.size(); i++)
						{
							if (Texture::textures[i]->name == textureFileName)
							{
								loadTexture = false;
							}
						}

						if (loadTexture)
						{
							texture = new Texture(textureFileName);
							texture->name = lTexture->GetName();
						}
					}
				}
			}
		}

		int count = node->GetChildCount();
		for (int i = 0; i < count; i++) 
		{
			LoadSupportedTexturesRecursive(node->GetChild(i));
		}
	}
}
