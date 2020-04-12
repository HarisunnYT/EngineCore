#include "PlayerController.h"
#include "InputSystem.h"
#include "Brick.h"

std::string PlayerController::componentName = "playercontroller";

PlayerController::PlayerController(float s)
{
	speed = s;
}

void PlayerController::Init()
{
	rigidbody = &entity->AddComponent<Rigidbody>();
	transform = &entity->GetComponent<Transform>();
	animator = &entity->GetComponent<Animator>();
	collider = &entity->GetComponent<Collider>();
}

void PlayerController::Update()
{
	if (growing)
	{
		timer = timer + EngineCore::deltaTime;;
		float normTime = timer / growDuration;

		if (normTime >= 1.0f)
		{
			entity->GetComponent<Rigidbody>().useGravity = true;
			growing = false;
		}
	}

	if (invincible)
	{
		timer = timer + EngineCore::deltaTime;;
		float normTime = timer / invincibleDuration;

		if (normTime >= 1.0f)
		{
			invincible = false;
			entity->GetComponent<Animator>().enabled = true;
		}
		else if (SDL_GetTicks() > enabledFlipFlopTime)
		{
			entity->GetComponent<Animator>().enabled = !entity->GetComponent<Animator>().enabled;
			enabledFlipFlopTime = SDL_GetTicks() + 100;
		}
	}

	if (growing || !hasInput)
		return;

	Vector2 velocity = rigidbody->GetVelocity();

	if (InputSystem::KeyHeld(SDL_SCANCODE_A))
	{
		if (accel > -speed)
			accel -= acceleration * EngineCore::deltaTime;

		if (inAir)
			animator->PlayAnimation(isBig ? "JumpLeftBig" : "JumpLeft");
		else
			animator->PlayAnimation(isBig ? "WalkLeftBig" : "WalkLeft");

		currentDirection = -1;
	}
	else if (InputSystem::KeyHeld(SDL_SCANCODE_D))
	{
		if (accel < speed)
			accel += acceleration * EngineCore::deltaTime;

		if (inAir)
			animator->PlayAnimation(isBig ? "JumpRightBig" : "JumpRight");
		else
			animator->PlayAnimation(isBig ? "WalkRightBig" : "WalkRight");

		currentDirection = 1;
	}
	else
	{
		accel = (accel * (1.0f - EngineCore::deltaTime)) + (0 * EngineCore::deltaTime);
		if (!inAir)
		{
			if (isBig)
				animator->PlayAnimation(currentDirection > 0 ? "IdleRightBig" : "IdleLeftBig");
			else
				animator->PlayAnimation(currentDirection > 0 ? "IdleRight" : "IdleLeft");
		}
	}

	if (InputSystem::KeyHeld(SDL_SCANCODE_SPACE) && velocity.y == 0)
	{
		velocity.y += jumpSpeed;
		jumping = true;
		inAir = true;

		entity->GetComponent<AudioSource>().Play(L"Assets/Audio/Jump.wav");
	}
	else if (!InputSystem::KeyHeld(SDL_SCANCODE_SPACE) || velocity.y >= maxJumpVelocity)
	{
		jumping = false;
	}

	if (jumping && InputSystem::KeyHeld(SDL_SCANCODE_SPACE) && velocity.y < maxJumpVelocity)
	{
		velocity.y = velocity.y + (jumpLerpSpeed * EngineCore::deltaTime);
	}

	if (inAir)
	{
		if (isBig)
			animator->PlayAnimation(currentDirection > 0 ? "JumpRightBig" : "JumpLeftBig");
		else
			animator->PlayAnimation(currentDirection > 0 ? "JumpRight" : "JumpLeft");
	}

	velocity.x = accel;

	rigidbody->SetVelocity(velocity);
}

void PlayerController::OnCollision(Hit* hit)
{
	if (hit->normal.y < -0.8f)
	{
		if ((hit->collider->Tag == "brick" || hit->collider->Tag == "invisibleBrick") && rigidbody->GetVelocity().y > 0)
		{
			if (isBig && hit->collider->entity->GetComponent<Brick>().brickType == 0)
			{
				hit->collider->entity->SetActive(false);
			}
			else
				hit->collider->entity->GetComponent<Brick>().Bump();
		}

		jumping = false;
	}
	else if (hit->normal.y > 0.8f && !jumping && !growing)
	{
		inAir = false;
		if (isBig)
			animator->PlayAnimation(currentDirection > 0 ? "WalkRightBig" : "WalkLeftBig");
		else
			animator->PlayAnimation(currentDirection > 0 ? "WalkRight" : "WalkLeft");
	}
}

void PlayerController::OnTrigger(Hit* hit)
{
	if (hit->collider->Tag == "invisibleBrick" && hit->normal.y < -0.8f && rigidbody->GetVelocity().y > 0)
	{
		hit->collider->entity->GetComponent<Collider>().Trigger = false;
		hit->collider->entity->GetComponent<Tile>().SetSource(Vector2(96.0f, 32.0f));
	}
}

void PlayerController::SetBig()
{
	growing = true;

	isBig = true;
	collider->SetSize(Vector2(17, 30));
	collider->SetOffset(Vector2(22, 5));

	entity->GetComponent<Animator>().PlayAnimation("Grow");
	entity->GetComponent<Rigidbody>().SetVelocity(Vector2(0, 0));
	entity->GetComponent<Rigidbody>().useGravity = false;
}

void PlayerController::SetSmall()
{
	isBig = false;
	collider->SetSize(Vector2(10, 17));
	collider->SetOffset(Vector2(32, 17));

	invincible = true;
	timer = 0;
}

void PlayerController::TakeDamage()
{
	if (invincible)
		return;

	if (isBig)
	{
		entity->GetComponent<AudioSource>().Play(L"Assets/Audio/Damaged.wav");
		SetSmall();
	}
	else
	{
		hasInput = false;

		collider->Trigger = true;
		rigidbody->SetVelocity(Vector2(0, 200));

		entity->GetComponent<AudioSource>().Play(L"Assets/Audio/Die.wav");
		entity->GetComponent<Animator>().PlayAnimation("Die");
	}
}

void PlayerController::Die()
{
}

std::string PlayerController::Parse()
{
	std::stringstream ss;
	{
		cereal::JSONOutputArchive oarchive(ss);
		oarchive(PlayerController::componentName, speed);
	}

	return ss.str();
}

bool PlayerController::TryParse(std::string value, Entity* entity)
{
	std::string name;
	float speed;

	std::stringstream ss(value);
	{
		cereal::JSONInputArchive oarchive(ss);
		oarchive(name, speed);
	}

	if (name == PlayerController::componentName)
	{
		entity->AddComponent<PlayerController>(speed);
		return true;
	}

	return false;
}
