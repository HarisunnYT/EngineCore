#include "..\Header Files\Camera.h"

Camera::Camera()
{
	transform = &AddComponent<Transform>(Vector3(EngineCore::screenSize.x, EngineCore::screenSize.y, 0));
	offset = { 0, 0, static_cast<int>(EngineCore::screenSize.x), static_cast<int>(EngineCore::screenSize.y) };
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
