#include "..\Header Files\Camera.h"
#include "InputSystem.h"

#include "3rdPartyLibs/Includes/GLM/gtx/transform.hpp"

Camera::Camera()
{
	offset = { 0, 0, static_cast<int>(EngineCore::screenSize.x), static_cast<int>(EngineCore::screenSize.y) };
}

void Camera::Update()
{
	if (InputSystem::MouseHeld(SDL_BUTTON_RIGHT))
	{
		Vector2 mouseDelta = InputSystem::MousePosition - previousMouseHeldPosition;
		glm::vec3 v = glm::vec3(EngineCore::camera->transform->eulerRotation.x, EngineCore::camera->transform->eulerRotation.y, EngineCore::camera->transform->eulerRotation.z);
		glm::vec3 toRotateAround = glm::cross(v, glm::vec3(0, 1, 0));
		glm::mat4 rotator = glm::rotate(-mouseDelta.x / rotationDamper, glm::vec3(0, 1, 0)) *
							glm::rotate(-mouseDelta.y / rotationDamper, toRotateAround);
		v = glm::mat3(rotator) * v;

		EngineCore::camera->transform->eulerRotation = Vector3(v.x, v.y, v.z);
	}

	if (InputSystem::KeyHeld(SDL_SCANCODE_W))
	{
		EngineCore::camera->transform->position += EngineCore::camera->transform->eulerRotation.Normalised() * movementSpeed;
	}
	if (InputSystem::KeyHeld(SDL_SCANCODE_S))
	{
		EngineCore::camera->transform->position += EngineCore::camera->transform->eulerRotation.Normalised() * -movementSpeed;
	}
	if (InputSystem::KeyHeld(SDL_SCANCODE_D))
	{
		Vector3 strafeDirection = EngineCore::camera->transform->eulerRotation.Normalised().Cross(Vector3::Up());
		EngineCore::camera->transform->position += strafeDirection * movementSpeed;
	}
	if (InputSystem::KeyHeld(SDL_SCANCODE_A))
	{
		Vector3 strafeDirection = EngineCore::camera->transform->eulerRotation.Normalised().Cross(Vector3::Up());
		EngineCore::camera->transform->position += strafeDirection * -movementSpeed;
	}

	previousMouseHeldPosition = InputSystem::MousePosition;
}

void Camera::Draw()
{
	Vector3 cameraPos = EngineCore::camera->transform->position;
	Vector3 euler = EngineCore::camera->transform->eulerRotation.Normalised();

	gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, cameraPos.x + euler.x, cameraPos.y + euler.y, cameraPos.z + euler.z, 0, 1, 0);
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
