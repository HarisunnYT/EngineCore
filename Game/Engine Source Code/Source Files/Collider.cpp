#include "Collider.h"
#include "Camera.h"

std::string Collider::componentName = "collider";

Collider::Collider(std::string tag, bool isTrigger)
{
	Tag = tag;
	Trigger = isTrigger;

	offset = Vector2(0, 0);
	size = Vector2(1, 1);
}

void Collider::Init()
{
	transform = &entity->GetComponent<Transform>();
	if (entity->HasComponent<SpriteRenderer>())
	{
		spriteRenderer = &entity->GetComponent<SpriteRenderer>();
	}
	if (entity->HasComponent<Rigidbody>())
	{
		rigidbody = &entity->GetComponent<Rigidbody>();
	}

	if (spriteRenderer != nullptr)
	{
		size.x = spriteRenderer->spriteSize.x;
		size.y = spriteRenderer->spriteSize.y;
	}
}

void Collider::Update()
{
	UpdateCollider();

	previousPosition = transform->GetPosition();
}

void Collider::DebugDraw()
{
	SDL_SetRenderDrawColor(EngineCore::Renderer, 0, 255, 0, 255);
	SDL_RenderDrawRect(EngineCore::Renderer, &collider);
	SDL_SetRenderDrawColor(EngineCore::Renderer, 99, 173, 255, 255);
}

void Collider::Physics()
{
}

void Collider::OnEnable()
{
	UpdateCollider();

	Collision::UpdateGrid(this);
}

void Collider::OnDisable()
{
	Collision::UpdateGrid(this);
}

void Collider::SetSize(Vector2 s)
{
	size = s;
	UpdateCollider();

	Collision::UpdateGrid(this);
}

void Collider::SetOffset(Vector2 o)
{
	offset = o;

	Collision::UpdateGrid(this);
}

const SDL_Rect Collider::GetCollider()
{
	UpdateCollider();

	return collider;
}

std::string Collider::Parse()
{
	std::stringstream ss;
	{
		cereal::JSONOutputArchive oarchive(ss);
		oarchive(Collider::componentName, Tag, offset.ToString(), size.ToString(), Trigger);
	}

	return ss.str();
}

bool Collider::TryParse(std::string value, Entity* entity)
{
	std::string name;
	std::string tag;
	std::string inOffset;
	std::string inSize;
	bool inTrigger;

	std::stringstream ss(value);
	{
		cereal::JSONInputArchive oarchive(ss);
		oarchive(name, tag, inOffset, inSize, inTrigger);
	}

	if (name == Collider::componentName)
	{
		entity->AddComponent<Collider>(tag, inTrigger);
		entity->GetComponent<Collider>().SetSize(Vector2::FromString(inSize));
		entity->GetComponent<Collider>().SetOffset(Vector2::FromString(inOffset));

		return true;
	}

	return false;
}

SDL_Rect Collider::RawCollider()
{
	UpdateCollider();

	SDL_Rect rect;
	rect.x = collider.x - EngineCore::camera->offset.x;
	rect.y = collider.y - EngineCore::camera->offset.y;
	rect.w = collider.w;
	rect.h = collider.h;

	return rect;
}

Vector2 Collider::GetMinBounds()
{
	return Vector2(static_cast<float>(collider.x), static_cast<float>(collider.y));
}

Vector2 Collider::GetMaxBounds()
{
	return Vector2(static_cast<float>(collider.x + collider.w), static_cast<float>(collider.y + collider.h));
}

Vector2 Collider::Centre()
{
	return Vector2(static_cast<float>(collider.x + (collider.w / 2)), static_cast<float>(collider.y + (collider.h / 2)));
}

void Collider::UpdateCollider()
{
	collider.w = static_cast<int>(size.x * transform->scale.x);
	collider.h = static_cast<int>(size.y * transform->scale.y);

	collider.x = static_cast<int>(transform->GetPosition().x + offset.x);
	collider.y = static_cast<int>(transform->GetPosition().y + offset.y);
}
