#include "Tile.h"
#include "Camera.h"

std::string Tile::componentName = "tile";

Tile::Tile(const char* p, Vector3 pos, Vector2 mSize, Vector2 mSource, float s)
{
	currentPath = p;
	size = mSize;
	source = mSource;

	texture = TextureManager::LoadTexture(p);

	ManualConstruction(pos, s);
}

Tile::~Tile()
{
	SDL_DestroyTexture(texture);
}

void Tile::ManualConstruction(Vector3 pos, float s)
{
	startingPosition = pos;
	startingScale = Vector2(s, s);

	SetRects();
}

void Tile::Init()
{
	transform = &entity->GetComponent<Transform>();
	transform->SetPosition(startingPosition);
	transform->scale = startingScale;

	if (entity->HasComponent<Collider>())
	{
		collider = &entity->GetComponent<Collider>();
		collider->SetSize(Vector2((float)sourceRect.w, (float)sourceRect.h));
	}
}

void Tile::Update()
{
	destinationRect.x = static_cast<int>(transform->GetPosition().x);
	destinationRect.y = static_cast<int>(transform->GetPosition().y);

	destinationRect.w = static_cast<int>(size.x * transform->scale.x);
	destinationRect.h = static_cast<int>(size.y * transform->scale.y);
}

void Tile::Draw()
{
	if (EngineCore::camera->InsideCameraView(&destinationRect))
	{
		TextureManager::Draw(texture, sourceRect, destinationRect);
	}
}

void Tile::SetSource(Vector2 s)
{
	source = s;
	SetRects();
}

std::string Tile::Parse()
{
	std::stringstream ss;
	{
		cereal::JSONOutputArchive oarchive(ss);
		oarchive(Tile::componentName, currentPath, size.ToString(), source.ToString());
	}

	return ss.str();
}

bool Tile::TryParse(std::string value, Entity* entity)
{
	std::string name;
	std::string inPath;
	std::string inSize;
	std::string inSource;

	std::stringstream ss(value);
	{
		cereal::JSONInputArchive oarchive(ss);
		oarchive(name, inPath, inSize, inSource);
	}

	if (name == Tile::componentName)
	{
		entity->AddComponent<Tile>(inPath.c_str(), entity->GetComponent<Transform>().GetRawPosition(), Vector2::FromString(inSize), Vector2::FromString(inSource), 1.0f);

		return true;
	}

	return false;
}

void Tile::SetRects()
{
	sourceRect.x = static_cast<int>(source.x);
	sourceRect.y = static_cast<int>(source.y);
	sourceRect.w = static_cast<int>(size.x);
	sourceRect.h = static_cast<int>(size.y);

	destinationRect.x = static_cast<int>(startingPosition.x);
	destinationRect.y = static_cast<int>(startingPosition.y);
	destinationRect.w = static_cast<int>(size.x * startingScale.x);
	destinationRect.h = static_cast<int>(size.y * startingScale.y);
}
