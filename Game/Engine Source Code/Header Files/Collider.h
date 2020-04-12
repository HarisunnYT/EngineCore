#pragma once

#ifndef COLLIDER_H
#define COLLIDER_H

#include "EngineCore.h"

class Transform;
class SpriteRenderer;
class Rigidbody;

class Collider : public Component
{
public:

	Collider() = default;
	Collider(std::string tag, bool isTrigger);

	void Init() override;
	void Update() override;
	void DebugDraw() override;
	void Physics() override;
	void OnEnable() override;
	void OnDisable() override;

	void SetSize(Vector2 size);
	void SetOffset(Vector2 offset);

	void UpdateCollider();

	const SDL_Rect GetCollider();

	std::string Parse() override;
	static bool TryParse(std::string value, Entity* entity);
	static std::string componentName;

	template<class Archive>
	void Serialize(Archive& archive)
	{
		archive(componentName, offset.ToString(), size.ToString(), Trigger);
	}

	std::string Tag;
	bool Trigger = false;

	SDL_Rect collider;

	SDL_Rect RawCollider();

	Vector2 GetMinBounds();
	Vector2 GetMaxBounds();
	Vector2 Centre();

private:


	Transform*			transform = nullptr;
	SpriteRenderer*		spriteRenderer = nullptr;
	Rigidbody*			rigidbody = nullptr;

	Vector2				offset;
	Vector2				size;

	Vector3				previousPosition;
};

#endif