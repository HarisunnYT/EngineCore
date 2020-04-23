#include "3rdPartyLibs/Includes/GL/glew.h"

#include "..\Header Files\Camera.h"
#include "InputSystem.h"

#include "3rdPartyLibs/Includes/GLM/gtx/transform.hpp"
#include <math.h>

Camera::Camera()
{
	offset = { 0, 0, static_cast<int>(EngineCore::screenSize.x), static_cast<int>(EngineCore::screenSize.y) };
}

void Camera::ViewUpdate()
{
	if (InputSystem::MouseHeld(SDL_BUTTON_RIGHT))
	{
		Vector2 mousePosition = InputSystem::MousePosition;

		float xoffset = (mousePosition.x - previousMouseHeldPosition.x) * mouseSensitivity;
		float yoffset = ( previousMouseHeldPosition.y - mousePosition.y) * mouseSensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		Vector3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		direction.Normalise();

		EngineCore::camera->entity->transform->eulerRotation = direction;
	}

	if (InputSystem::KeyHeld(SDL_SCANCODE_W))
	{
		EngineCore::camera->entity->transform->position += EngineCore::camera->entity->transform->eulerRotation.Normalised() * movementSpeed;
	}
	if (InputSystem::KeyHeld(SDL_SCANCODE_S))
	{
		EngineCore::camera->entity->transform->position += EngineCore::camera->entity->transform->eulerRotation.Normalised() * -movementSpeed;
	}
	if (InputSystem::KeyHeld(SDL_SCANCODE_D))
	{
		Vector3 strafeDirection = Vector3::Cross(EngineCore::camera->entity->transform->eulerRotation.Normalised(), Vector3::Up());
		EngineCore::camera->entity->transform->position += strafeDirection * movementSpeed;
	}
	if (InputSystem::KeyHeld(SDL_SCANCODE_A))
	{
		Vector3 strafeDirection = Vector3::Cross(EngineCore::camera->entity->transform->eulerRotation.Normalised(), Vector3::Up());
		EngineCore::camera->entity->transform->position += strafeDirection * -movementSpeed;
	}

	previousMouseHeldPosition = InputSystem::MousePosition;

	Vector3 cameraPos = EngineCore::camera->entity->transform->position;
	Vector3 euler = EngineCore::camera->entity->transform->eulerRotation.Normalised();

	gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, cameraPos.x + euler.x, cameraPos.y + euler.y, cameraPos.z + euler.z, 0, 1, 0);
	//gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, cameraPos.x, cameraPos.y, cameraPos.z - 1, 0, 1, 0);
}

void Camera::Update()
{		
}

void Camera::Draw()
{	
}

bool Camera::InsideCameraView(SDL_Rect* rect)
{
	inputTopLeft = Vector2(static_cast<float>(rect->x), static_cast<float>(rect->y));
	inputBottomLeft = Vector2(static_cast<float>(rect->x), static_cast<float>(rect->y + rect->h));

	inputTopRight = Vector2(static_cast<float>(rect->x + rect->w), static_cast<float>(rect->y));
	inputBottomRight = Vector2(static_cast<float>(rect->x + rect->w), static_cast<float>(rect->y + rect->h));

	if (inputTopRight.x >= screenTopLeft.x && inputTopLeft.x <= screenTopRight.x &&
		inputBottomLeft.y >= screenTopLeft.y && inputTopRight.y <= screenBottomRight.y)
	{
		return true;
	}

	return false;
}
