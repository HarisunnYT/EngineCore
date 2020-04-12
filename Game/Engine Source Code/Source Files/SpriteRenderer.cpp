#include "SpriteRenderer.h"
#include "TextureManager.h"
#include "Transform.h"
#include "Camera.h"

std::string SpriteRenderer::componentName = "spriterenderer";

SpriteRenderer::SpriteRenderer(const char* p, Vector2 size)
{
	spriteSize = Vector2((float)size.x, (float)size.y);
	currentPath = p;
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Init()
{
	transform = &entity->GetComponent<Transform>();

	sourceRect.x = 0;
	sourceRect.y = 0;

	sourceRect.w = (int)spriteSize.x;
	sourceRect.h = (int)spriteSize.y;

	destinationRect.w = (int)(spriteSize.x * transform->scale.x);
	destinationRect.h = (int)(spriteSize.y * transform->scale.y);

	SetTexture(currentPath.c_str());
}

void SpriteRenderer::Draw()
{
	if (!enabled)
		return;

	destinationRect.x = static_cast<int>(transform->GetPosition().x);
	destinationRect.y = static_cast<int>(transform->GetPosition().y);

	destinationRect.w = static_cast<int>(spriteSize.x * transform->scale.x);
	destinationRect.h = static_cast<int>(spriteSize.y * transform->scale.y);

	if (EngineCore::camera->InsideCameraView(&destinationRect))
	{
		TextureManager::Draw(texture, sourceRect, destinationRect);
	}
}

void SpriteRenderer::SetTexture(const char* path)
{
	texture = TextureManager::LoadTexture(path);
	currentPath = path;
}

std::string SpriteRenderer::Parse()
{
	std::stringstream ss;
	{
		cereal::JSONOutputArchive oarchive(ss);
		oarchive(SpriteRenderer::componentName, currentPath, spriteSize.ToString());
	}

	return ss.str();
}

bool SpriteRenderer::TryParse(std::string value, Entity* entity)
{
	std::string name;
	std::string inPath;
	std::string inSize;

	std::stringstream ss(value);
	{
		cereal::JSONInputArchive oarchive(ss);
		oarchive(name, inPath, inSize);
	}

	if (name == SpriteRenderer::componentName)
	{
		entity->AddComponent<SpriteRenderer>(inPath.c_str(), Vector2::FromString(inSize));

		return true;
	}

	return false;
}
