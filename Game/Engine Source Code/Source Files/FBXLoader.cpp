#include "..\Header Files\FBXLoader.h"

using namespace fbxsdk;

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
}

void FBXLoader::DrawModel(Object* object, long time)
{
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

	scene = FbxScene::Create(sdkManager, fileName.data());
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

	delete &sdkManager;

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
				FbxLayerElementTexture* lTextureLayer = pMesh->GetLayer(0)->GetTextures(FbxLayerElement::EType::eUnknown);
				
				material = new MaterialFBX();
				
				if (lMaterialLayer != nullptr && lMaterialLayer->mDirectArray->GetCount() > 0)
				{
					lMaterial = lMaterialLayer->mDirectArray->GetAt(0);

					FbxProperty diffuse = lMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
					FbxAnimCurveNode* diffuseAnimCurveNode = diffuse.GetCurveNode();

					material->Kd[0] = diffuseAnimCurveNode->GetCurve(0, 0)->GetValue();
					material->Kd[1] = diffuseAnimCurveNode->GetCurve(1, 0)->GetValue();
					material->Kd[3] = diffuseAnimCurveNode->GetCurve(2, 0)->GetValue();
				}
			}
			break;
		}
	}

	for (int i = 0; i < node->GetChildCount(); i++)
	{
		LoadContent(scene, node->GetChild(i), nextList);
	}
}

void FBXLoader::ComputeTransformation(fbxsdk::FbxMatrix& globalPosition, fbxsdk::FbxMesh* pMesh, fbxsdk::FbxVector4* vertexArray, MeshFBX* mesh, fbxsdk::FbxTime& time, fbxsdk::FbxPose* pose)
{
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
