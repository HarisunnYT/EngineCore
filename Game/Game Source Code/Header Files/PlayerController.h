#pragma once

#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "Components.h"
#include "Game.h"
#include "ECS.h"
#include "Animator.h"

class PlayerController : public Component
{
public:

	PlayerController(float speed = 1);

	void Init() override;
	void Update() override;

	void OnCollision(Hit* hit) override;
	void OnTrigger(Hit* hit) override;

	void SetBig();
	void SetSmall();

	void TakeDamage();
	void Die();

	std::string Parse() override;
	static bool TryParse(std::string value, Entity* entity);
	static std::string componentName;

	template<class Archive>
	void Serialize(Archive& archive)
	{
		archive(componentName, speed);
	}

private:

	bool		hasInput = true;

	float		speed = 100.0f;
	float		acceleration = 100.0f;
	float		accel = 0.0f;

	float		jumpSpeed = 200.0f;
	float		jumpLerpSpeed = 1000.0f;
	float		maxJumpVelocity = 340.0f;

	bool		jumping = false;
	bool		inAir = false;
	bool		isBig = false;

	bool		growing = false;
	float		growDuration = 1.0f;

	bool		invincible = false;
	float		invincibleDuration = 2.0f;
	float		enabledFlipFlopTime = 0.0f;

	float		timer = 0;

	int			currentDirection = 0;

	Transform*	transform = nullptr;
	Rigidbody*	rigidbody = nullptr;
	Animator*	animator = nullptr;
	Collider*	collider = nullptr;

};

#endif