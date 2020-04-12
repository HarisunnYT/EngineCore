#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <iostream>
#include <bitset>
#include <array>
#include "ECS.h"

#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

class Component;

using ComponentID = std::size_t;

inline ComponentID GetComponentTypeID()
{
	static ComponentID lastID = 0;
	return lastID++;
}

template <typename T> inline ComponentID GetComponentTypeID() noexcept
{
	static ComponentID typeID = GetComponentTypeID();
	return typeID;
}

constexpr std::size_t MaxComponents = 32;

using ComponentBitSet = std::bitset<MaxComponents>;
using ComponentArray = std::array<Component*, MaxComponents>;

class Transform;
class Hit;
class Entity
{
public:

	Entity();
	~Entity();

	virtual void Update();
	virtual void Draw();
	virtual void Destroy();
	virtual void Physics();
	virtual void LateUpdate();
	virtual void DebugDraw();

	virtual void OnEnable();
	virtual void OnDisable();

	virtual void OnCollision(Hit* hit);
	virtual void OnTrigger(Hit* hit);

	bool IsActive() const;
	void SetActive(bool active);

	void SaveToDisk(const char* path);

	template <typename T> bool HasComponent() const
	{
		return componentBitSet[GetComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs> T& AddComponent(TArgs&&... mArgs)
	{
		if (!HasComponent<T>())
		{
			T* c(new T(std::forward<TArgs>(mArgs)...));
			c->entity = this;
			std::unique_ptr<Component> uPtr{ c };
			components.emplace_back(std::move(uPtr));

			componentArray[GetComponentTypeID<T>()] = c;
			componentBitSet[GetComponentTypeID<T>()] = true;

			c->Init();
			return *c;
		}
		else
		{
			return GetComponent<T>();
		}
	}

	template <typename T> T& GetComponent() const
	{
		auto ptr(componentArray[GetComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

	Transform* transform;

private:

	bool active = true;

	std::vector<std::unique_ptr<Component>> components;

	ComponentArray		componentArray;
	ComponentBitSet		componentBitSet;
};

#endif